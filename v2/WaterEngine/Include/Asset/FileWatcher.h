// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include <functional>

namespace we
{
	// =============================================================================
	// File Change Callback
	// =============================================================================
	using FileChangeCallback = std::function<void(const string& FilePath)>;

	// =============================================================================
	// File Watcher
	// Monitors directories for file changes (Debug build only)
	// =============================================================================
	class FileWatcher
	{
	public:
		FileWatcher();
		~FileWatcher();

		// Start watching a directory
		void WatchDirectory(const string& DirectoryPath);
		
		// Stop watching
		void Stop();
		
		// Set callback for file changes
		void SetCallback(FileChangeCallback Callback) { m_Callback = Callback; }
		
		// Update - call this each frame to process changes
		void Update();
		
		// Check if watcher is active
		bool IsWatching() const { return m_bIsRunning; }

	private:
		string m_WatchPath;
		FileChangeCallback m_Callback;
		bool m_bIsRunning = false;
		
		// Platform-specific implementation
		class Impl;
		unique<Impl> m_Impl;
	};
}
