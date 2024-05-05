-- File: lua/custom/plugins/nvim-tree.lua

return {
  'DaikyXendo/nvim-tree.lua',
  event = 'VeryLazy',
  config = function()
    vim.g.loaded_netrw = 1
    vim.g.loaded_netrwPlugin = 1
    require('nvim-tree').setup {}
  end,
}
