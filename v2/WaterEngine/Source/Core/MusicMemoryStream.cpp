// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Core/MusicMemoryStream.h"

namespace we
{
    MusicMemoryStream::MusicMemoryStream()
        : ReadPosition(0)
    {
    }

    MusicMemoryStream::MusicMemoryStream(shared<vector<uint8>> Data)
        : DataBuffer(std::move(Data))
        , ReadPosition(0)
    {
    }

    void MusicMemoryStream::SetData(shared<vector<uint8>> Data)
    {
        DataBuffer = std::move(Data);
        ReadPosition = 0;
    }

    std::optional<std::size_t> MusicMemoryStream::read(void* data, std::size_t size)
    {
        if (!DataBuffer || DataBuffer->empty())
            return std::nullopt;

        // Calculate how much we can actually read
        std::size_t available = DataBuffer->size() - ReadPosition;
        std::size_t toRead = std::min(size, available);

        if (toRead == 0)
            return 0;

        // Copy data
        std::memcpy(data, DataBuffer->data() + ReadPosition, toRead);
        ReadPosition += toRead;

        return toRead;
    }

    std::optional<std::size_t> MusicMemoryStream::seek(std::size_t position)
    {
        if (!DataBuffer)
            return std::nullopt;

        ReadPosition = std::min(position, DataBuffer->size());
        return ReadPosition;
    }

    std::optional<std::size_t> MusicMemoryStream::tell()
    {
        return ReadPosition;
    }

    std::optional<std::size_t> MusicMemoryStream::getSize()
    {
        if (!DataBuffer)
            return 0;
        return DataBuffer->size();
    }
}
