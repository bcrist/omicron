demo 'omicron' {
   app {
      icon 'icon/bengine.ico',
      src {
         'src/*.cpp',
         'src/*.c'
      },
      link_project {
          'platform',
          'core-id-with-names',
          'util-fs',
          'util-prng',
          'cli',
          'glfw',
          'glew',
          'core'
      }
   }
}
