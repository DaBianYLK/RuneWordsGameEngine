#pragma once

#define SafeDelete(ptr) { delete ptr; ptr = nullptr; }
