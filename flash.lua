-- File: lua/custom/plugins/flash.lua

return {
  'folke/flash.nvim',
  event = 'VeryLazy',
  -- Optional dependency
  config = function()
    require('flash').setup {}
  end,
}
