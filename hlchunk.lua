-- File: lua/custom/plugins/hlchunk.lua

return {
  'shellRaining/hlchunk.nvim',
  event = { 'UIEnter' },
  config = function()
    require('hlchunk').setup {
      chunk = {
        chars = {
          right_arrow = '',
        },
      },
      indent = {
        enable = false,
      },
      line_num = {
        enable = true,
        style = {
          hibiscus = '#806d9c',
        },
      },
      blank = {
        enable = false,
      },
    }
  end,
}
