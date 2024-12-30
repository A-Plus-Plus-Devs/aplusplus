import { defineConfig } from 'vitepress'

export default defineConfig({
  title: "A++",
  description: "A modern, expressive, and easy-to-learn programming language",
  srcDir: '.',
  outDir: './.vitepress/dist',
  base: '/aplusplus',
  cleanUrls: true,
  head: [
    ['link', { rel: 'icon', href: '/logo.svg' }],
    ['meta', { name: 'theme-color', content: '#646cff' }]
  ],
  themeConfig: {
    logo: '/logo.svg',
    nav: [
      { text: 'Guide', link: '/guide/getting-started' },
      { text: 'Reference', link: '/reference/syntax' },
      { text: 'Examples', link: '/examples/hello-world' },
      { text: 'GitHub', link: 'https://github.com/A-Plus-Plus-Devs/aplusplus' }
    ],
    sidebar: {
      '/': [
        {
          text: 'Getting Started',
          items: [
            { text: 'Installation', link: '/guide/installation' },
            { text: 'First Program', link: '/guide/first-program' }
          ]
        },
        {
          text: 'Core Concepts',
          items: [
            { text: 'Control Flow', link: '/guide/control-flow' }
          ]
        },
        {
          text: 'Language Reference',
          items: [
            { text: 'Syntax', link: '/reference/syntax' },
            { text: 'Language Basics', link: '/reference/language-basics' },
          ]
        },
        {
          text: 'Development',
          items: [
            { text: 'Version Management', link: '/guide/versioning' },
            { text: 'Testing', link: '/guide/testing' },
          ]
        },
        {
          text: 'Examples',
          items: [
            { text: 'Hello World', link: '/examples/hello-world' },
          ]
        },
        {
          text: 'Advanced Topics',
          items: [
            { text: 'Classes', link: '/guide/classes' },
            { text: 'Error Handling', link: '/guide/error-handling' },
          ]
        }
      ]
    },
    socialLinks: [
      { icon: 'github', link: 'https://github.com/A-Plus-Plus-Devs/aplusplus' }
    ],
    footer: {
      message: 'Released under the MIT License.',
      copyright: 'Copyright 2024 A++ Team'
    }
  }
})
