-- File: lua/custom/plugins/noice.lua

return {
  'folke/noice.nvim',
  event = 'VeryLazy',
  dependencies = { 'MunifTanjim/nui.nvim', 'rcarriga/nvim-notify' },
  config = function()
    local icons = require 'custom.icons'
    require('noice').setup {
      lsp = {
        override = {
          ['vim.lsp.util.convert_input_to_markdown_lines'] = true,
          ['vim.lsp.util.stylize_markdown'] = true,
          ['cmp.entry.get_documentation'] = true,
        },
      },
      cmdline = {
        enabled = true,
        format = {
          help = {
            title = '',
          },
          input = {
            title = '',
          },
          cmdline = {
            icon = icons.ui.Command,
            title = '',
          },
          search_down = {
            icon = icons.ui.Search,
            title = '',
          },
          search_up = {
            icon = icons.ui.Search,
            title = '',
          },
        },
      },
      presets = {
        bottom_search = false,
        command_palette = false,
        long_message_to_split = true,
        inc_rename = true,
        lsp_doc_border = false,
      },
    }
  end,
}
