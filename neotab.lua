-- File: lua/custom/plugins/neotab.lua

return {
  'kawre/neotab.nvim',
  event = 'InsertEnter',
  config = function()
    require('neotab').setup {
      tabkey = '<Tab>',
      act_as_tab = true,
      behavior = 'nested',
      pairs = {
        { open = '(', close = ')' },
        { open = '[', close = ']' },
        { open = '{', close = '}' },
        { open = "'", close = "'" },
        { open = '"', close = '"' },
        { open = '`', close = '`' },
        { open = '<', close = '>' },
      },
      exclude = {},
      smart_punctuators = {
        enabled = true,
        semicolon = {
          enabled = false,
          ft = { 'cs', 'c', 'cpp', 'java' },
        },
        escape = {
          enabled = true,
          triggers = { ---@type table<string, ntab.trigger>
            [','] = {
              pairs = {
                { open = "'", close = "'" },
                { open = '"', close = '"' },
              },
              format = '%s ', -- ", "
              ft = { 'c' },
            },
          },
        },
      },
    }
  end,
}
