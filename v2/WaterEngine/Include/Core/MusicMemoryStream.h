// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include <SFML/System/InputStream.hpp>
#include <optional>

namespace we
{
    // =========================================================================
    // MusicMemoryStream - Allows SFML Music to stream from memory buffer
    // =========================================================================
    class MusicMemoryStream : public sf::InputStream
    {
    public:
        MusicMemoryStream();
        explicit MusicMemoryStream(shared<vector<uint8>> Data);

        void SetData(shared<vector<uint8>> Data);

        // sf::InputStream implementation
        std::optional<std::size_t> read(void* data, std::size_t size) override;
        std::optional<std::size_t> seek(std::size_t position) override;
        std::optional<std::size_t> tell() override;
        std::optional<std::size_t> getSize() override;

    private:
        shared<vector<uint8>> DataBuffer;
        std::size_t ReadPosition = 0;
    };
}
