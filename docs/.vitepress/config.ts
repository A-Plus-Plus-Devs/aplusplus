import { defineConfig } from 'vitepress'

export default defineConfig({
  title: "A++ Programming Language",
  description: "A modern, expressive, and easy-to-learn programming language",
  themeConfig: {
    logo: '/logo.svg',
    nav: [
      { text: 'Guide', link: '/guide/getting-started' },
      { text: 'Reference', link: '/reference/syntax' },
      { text: 'Examples', link: '/examples/hello-world' },
      { text: 'GitHub', link: 'https://github.com/A-Plus-Plus-Devs/aplusplus' }
    ],
    sidebar: {
      '/guide/': [
        {
          text: 'Introduction',
          items: [
            { text: 'Getting Started', link: '/guide/getting-started' },
            { text: 'Installation', link: '/guide/installation' },
            { text: 'First Program', link: '/guide/first-program' }
          ]
        },
        {
          text: 'Core Concepts',
          items: [
            { text: 'Variables & Types', link: '/guide/variables-types' },
            { text: 'Control Flow', link: '/guide/control-flow' },
            { text: 'Functions', link: '/guide/functions' },
            { text: 'Error Handling', link: '/guide/error-handling' }
          ]
        }
      ],
      '/reference/': [
        {
          text: 'Language Reference',
          items: [
            { text: 'Syntax', link: '/reference/syntax' },
            { text: 'Types', link: '/reference/types' },
            { text: 'Operators', link: '/reference/operators' },
            { text: 'Keywords', link: '/reference/keywords' },
            { text: 'Built-in Functions', link: '/reference/built-ins' }
          ]
        }
      ]
    },
    socialLinks: [
      { icon: 'github', link: 'https://github.com/A-Plus-Plus-Devs/aplusplus' }
    ],
    footer: {
      message: 'Released under the MIT License.',
      copyright: 'Copyright © 2024-present A++ Team'
    }
  }
})
