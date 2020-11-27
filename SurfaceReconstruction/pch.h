#ifndef PRECOMPILED_HEADER
#define PRECOMPILED_HEADER

#include "config.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp> 

#include <cmath>
#include <vector>
#include <list>
#include <string>
#include <memory>
#include <stack>
#include <algorithm>

#include <stdlib.h>
#include <cstdlib>
#include <cstddef>

#if defined(LINUX)
#include <unistd.h>
#include <dlfcn.h>
#elif defined(WINDOWS)
#define NOMINMAX
#include <windows.h>
#endif

#include <iostream>
#include <fstream>
#include <sstream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#endif //PRECOMPILED_HEADER