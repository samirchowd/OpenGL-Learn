project('OpenGL-Learn', ['cpp', 'c'],
  version : '0.1.0',
  default_options : ['warning_level=3', 'cpp_std=c++17'])

compiler = meson.get_compiler('cpp')
openGLDep = dependency('gl')
glewDep = dependency('glew', required: false)
gluLDep = dependency('glu', required: false)
glutDep = dependency('glut', required: false)
glfwDep = dependency('glfw3', required: false)
threadDep = dependency('threads', required: false)
XrandrDep = dependency('Xrandr', required: false)
XiDep = dependency('Xi', required: false)
dlDep = dependency('dl', required: false)
x11Dep = dependency('x11', required: false)
Xxf86vmDep = dependency('Xxf86vm', required :false)
XineramaDep = dependency('Xinerama', required: false)
XcursorDep = dependency('Xcursor', required: false)



inc = include_directories('/usr/local/include', 'include')
executable('OpenGL-Learn', sources: ['src/shader.cpp', 'src/triangle.cpp','/usr/local/src/glad.c'],
          include_directories : inc,
          dependencies: [openGLDep, glfwDep,])
