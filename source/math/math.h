#if !defined(MATH_H)
#define MATH_H
#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

#include <platform/platform.h>

#define X                                           (0U)
#define Y                                           (1U)
#define Z                                           (2U)
#define W                                           (3U)

typedef float cvec2f_t[2]; 
typedef int32_t cvec2i_t[2]; 
typedef uint32_t cvec2u_t[2]; 

typedef float cvec3f_t[3]; 
typedef int32_t cvec3i_t[3]; 
typedef uint32_t cvec3u_t[3];
 
typedef float cvec4f_t[4]; 
typedef int32_t cvec4i_t[4]; 
typedef uint32_t cvec4u_t[4]; 

#if defined(__cplusplus)
}
#endif // __cplusplus
#endif // MATH_H