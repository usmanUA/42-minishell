-- File: lua/custom/plugins/neoscroll.lua

return {
  'karb94/neoscroll.nvim',
  config = function()
    vim.cmd [[
  nnoremap <C-j> <C-D>
  vnoremap <C-j> <C-D>

  nnoremap <C-k> <C-U>
  vnoremap <C-k> <C-U>
]]
    require('neoscroll').setup {
      easy_function = 'quadratic',
      mappings = { '<C-u>', '<C-d>', '<C-b>', '<C-f>', '<C-y>', '<C-e>', 'zt', 'zz', 'zb', 'C-k', 'C-j' },
      hide_cursor = true,
      stop_eof = true,
      respect_scrolloff = false,
      cursor_scrolls_alone = true,
      easing_function = nil,
      pre_hook = nil,
      post_hook = nil,
      performance_mode = false,
    }
    local t = {}
    -- Syntax: t[keys] = {function, {function arguments}}
    -- Use the "sine" easing function
    t['<C-u>'] = { 'scroll', { '-vim.wo.scroll', 'true', '350', [['sine']] } }
    t['<C-d>'] = { 'scroll', { 'vim.wo.scroll', 'true', '350', [['sine']] } }
    -- Use the "circular" easing function
    t['<C-b>'] = { 'scroll', { '-vim.api.nvim_win_get_height(0)', 'true', '500', [['circular']] } }
    t['<C-f>'] = { 'scroll', { 'vim.api.nvim_win_get_height(0)', 'true', '500', [['circular']] } }
    -- Pass "nil" to disable the easing animation (constant scrolling speed)
    t['<C-y>'] = { 'scroll', { '-0.10', 'false', '100', nil } }
    t['<C-e>'] = { 'scroll', { '0.10', 'false', '100', nil } }
    -- When no easing function is provided the default easing function (in this case "quadratic") will be used
    t['zt'] = { 'zt', { '300' } }
    t['zz'] = { 'zz', { '300' } }
    t['zb'] = { 'zb', { '300' } }

    require('neoscroll.config').set_mappings(t)
  end,
}
