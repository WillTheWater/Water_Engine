// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include <spdlog/spdlog.h>

#define LOG(...) SPDLOG_INFO(__VA_ARGS__);
#define WARNING(...) SPDLOG_WARN(__VA_ARGS__);
#define ERROR(...) SPDLOG_ERROR(__VA_ARGS__);

// =========================== EXAMPLES ===========================
//	// 1. Simple string literal
//		LOG("Engine initialized successfully.");
//	
//	// 2. Fundamental types (int, float, double, bool)
//		int version = 2;
//		float delta = 0.016f;
//		LOG("Version: {}, Delta: {}", version, delta);
//	
//	// 3. Boolean values (renders as "true" or "false" in std::format)
//		bool isRunning = true;
//		LOG("Engine status: {}", isRunning);
//	
//	// 4. Hexadecimal formatting (useful for memory addresses or colors)
//		int hexValue = 0xFFAABB;
//		LOG("Clear color: {:x}", hexValue); // Outputs: ffaabb
//	
//	// 5. Binary formatting
//		LOG("Bitmask: {:b}", 0b10101010); // Outputs: 10101010
//	
//	// 6. Precision control for floating point (fixed to 2 decimal places)
//		double pi = 3.1415926535;
//		LOG("Pi restricted: {:.2f}", pi); // Outputs: 3.14
//	
//	// 7. Padding and alignment (Width of 10, right aligned)
//		LOG("Score: {:>10}", 500);
//	
//	// 8. Named arguments (If supported by your compiler's std::format implementation)
//	// Note: Some older std::format versions require positional indices instead: {0}
//		LOG("Player {} has {} health remaining.", "Will", 100);
//	
//	// 9. Standard containers (if your compiler supports C++23/26 ranges formatting)
//	// Otherwise, you may need to log specific elements:
//		std::vector<int> levels = { 1, 2, 3 };
//		LOG("Current level: {}", levels[0]);
//	
//	// 10. Memory addresses (Pointers)
//		void* ptr = this;
//		LOG("Object address: {}", ptr);
//	
//	// 11. Escaping curly braces (use double braces)
//		LOG("The format syntax uses {{}} to inject variables.");
//	
//	// 12. Using LOG in a conditional or error check
//		if (ptr == nullptr) {
//		    ERROR("Null pointer detected at line {}", __LINE__);
//		}
//	
//	// 13. Highlighting specific values with signs
//		LOG("Temperature change: {:+}", -5); // Forces the + or - sign to show
//	
//	// 14. Character logging
//		char grade = 'A';
//		LOG("Asset Load Status: [{}]", grade);
//	
//	// 15. Complex manual formatting for coordinates
//		int x = 1920, y = 1080;
//		LOG("Resolution: {}x{}", x, y);