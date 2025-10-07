project('OpenGL-Learn', ['cpp', 'c'],
  version : '0.1.0',
  default_options : ['warning_level=3', 'cpp_std=c++20'])

compiler = meson.get_compiler('cpp')
openGLDep = dependency('gl')
glewDep = dependency('glew', required: false)
gluLDep = dependency('glu', required: false)
glutDep = dependency('glut', required: false)
glfwDep = dependency('glfw3', required: false)
glmDep = dependency('glm', required: false)
threadDep = dependency('threads', required: false)
XrandrDep = dependency('Xrandr', required: false)
XiDep = dependency('Xi', required: false)
dlDep = dependency('dl', required: false)
x11Dep = dependency('x11', required: false)
Xxf86vmDep = dependency('Xxf86vm', required :false)
XineramaDep = dependency('Xinerama', required: false)
XcursorDep = dependency('Xcursor', required: false)



inc = include_directories('/usr/local/include', 'include')

# Copy all shader files on every build
shader_sources = run_command('find', meson.current_source_dir() + '/shaders', '-name', '*.glsl', '-type', 'f', check: true)
copy_shaders = custom_target('copy_shaders',
  input: files(shader_sources.stdout().strip().split('\n')),
  output: ['shaders'],
  command: ['cp', '-r', meson.current_source_dir() + '/shaders', meson.current_build_dir()],
  build_always_stale: true,
  build_by_default: true
)

# Copy materials.json file on every build
copy_materials = custom_target('copy_materials',
  input: files('materials.json'),
  output: ['materials.json'],
  command: ['cp', meson.current_source_dir() + '/materials.json', meson.current_build_dir()],
  build_always_stale: true,
  build_by_default: true
)

# Copy all texture files on every build
texture_sources = run_command('find', meson.current_source_dir() + '/textures', '-name', '*.png', '-o', '-name', '*.jpg', '-o', '-name', '*.jpeg', '-type', 'f', check: true)
copy_textures = custom_target('copy_textures',
  input: files(texture_sources.stdout().strip().split('\n')),
  output: ['textures'],
  command: ['cp', '-r', meson.current_source_dir() + '/textures', meson.current_build_dir()],
  build_always_stale: true,
  build_by_default: true
)

executable('OpenGL-Learn', sources: ['src/Shader.cpp', 'src/VertexBuffer.cpp', 'src/ElementBuffer.cpp', 'src/Texture.cpp', 'src/Camera.cpp', 'src/main.cpp', '/usr/local/src/glad.c'],
          include_directories : inc,
          dependencies: [openGLDep, glfwDep, glmDep])
