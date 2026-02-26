// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Asset/FileWatcher.h"
#include "Utility/Log.h"
#include <string>

// Platform-specific includes
#ifdef _WIN32
    #include <windows.h>
#endif

namespace we
{
	// =============================================================================
	// Windows Implementation
	// =============================================================================
	#ifdef _WIN32
	class FileWatcher::Impl
	{
	public:
		HANDLE hDirectory = INVALID_HANDLE_VALUE;
		OVERLAPPED Overlapped = {};
		vector<uint8> Buffer;
		bool bPendingRead = false;
	};
	#endif

	FileWatcher::FileWatcher()
		: m_Impl(make_unique<Impl>())
	{
	}

	FileWatcher::~FileWatcher()
	{
		Stop();
	}

	void FileWatcher::WatchDirectory(const string& DirectoryPath)
	{
		#ifdef _WIN32
		m_Impl->hDirectory = CreateFileA(
			DirectoryPath.c_str(),
			FILE_LIST_DIRECTORY,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			nullptr,
			OPEN_EXISTING,
			FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
			nullptr
		);

		if (m_Impl->hDirectory == INVALID_HANDLE_VALUE)
		{
			LOG("Failed to watch directory: {}", DirectoryPath);
			return;
		}

		m_WatchPath = DirectoryPath;
		m_bIsRunning = true;
		m_Impl->Buffer.resize(4096);
		
		LOG("Watching directory: {}", DirectoryPath);
		#else
			(void)DirectoryPath;
			LOG_WARNING("FileWatcher not implemented for this platform");
		#endif
	}

	void FileWatcher::Stop()
	{
		#ifdef _WIN32
		if (m_Impl->hDirectory != INVALID_HANDLE_VALUE)
		{
			CancelIo(m_Impl->hDirectory);
			CloseHandle(m_Impl->hDirectory);
			m_Impl->hDirectory = INVALID_HANDLE_VALUE;
		}
		#endif
		
		m_bIsRunning = false;
	}

	void FileWatcher::Update()
	{
		#ifdef _WIN32
		if (!m_bIsRunning || m_Impl->hDirectory == INVALID_HANDLE_VALUE)
			return;

		if (!m_Impl->bPendingRead)
		{
			ZeroMemory(&m_Impl->Overlapped, sizeof(OVERLAPPED));
			
			BOOL success = ReadDirectoryChangesW(
				m_Impl->hDirectory,
				m_Impl->Buffer.data(),
				static_cast<DWORD>(m_Impl->Buffer.size()),
				TRUE,  // Watch subtree
				FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_FILE_NAME,
				nullptr,
				&m_Impl->Overlapped,
				nullptr
			);

			if (success)
			{
				m_Impl->bPendingRead = true;
			}
		}

		// Check if read completed
		DWORD bytesTransferred = 0;
		BOOL result = GetOverlappedResult(m_Impl->hDirectory, &m_Impl->Overlapped, &bytesTransferred, FALSE);
		
		if (result && bytesTransferred > 0)
		{
			m_Impl->bPendingRead = false;
			
			FILE_NOTIFY_INFORMATION* info = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(m_Impl->Buffer.data());
			
			while (info)
			{
				std::wstring fileName(info->FileName, info->FileNameLength / sizeof(WCHAR));
				string narrowFileName(fileName.begin(), fileName.end());
				
				if (m_Callback)
				{
					m_Callback(narrowFileName);
				}

				if (info->NextEntryOffset == 0)
					break;
				
				info = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(
					reinterpret_cast<uint8_t*>(info) + info->NextEntryOffset
				);
			}
		}
		#endif
	}
}
