#include <stdio.h>

namespace we
{
	#define LOG(Message, ...) printf(Message "\n", ##__VA_ARGS__);
}