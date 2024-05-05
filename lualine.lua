-- File: lua/custom/plugins/lualine.lua

return {
  'nvim-lualine/lualine.nvim',
  dependencies = { 'nvim-tree/nvim-web-devicons' },
  config = function()
    local colors = {
      red = '#ca1243',
      white = '#f3f3f3',
      light_green = '#83a598',
      orange = '#fe8019',
      green = '#8ec07c',
      blue = '#268bd2', -- A deep blue color
      purple = '#d3869b', -- A rich purple color
      yellow = '#fabd2f', -- A bright yellow color
      pink = '#d65d0e', -- A vibrant pink color
    }

    local theme = {
      normal = {
        a = { fg = colors.white, bg = colors.yellow },
        b = { fg = colors.white, bg = colors.purple },
        c = { fg = colors.blue, bg = colors.white },
        z = { fg = colors.white, bg = colors.yellow },
      },
      insert = { a = { fg = colors.orange, bg = colors.light_green } },
      visual = { a = { fg = colors.black, bg = colors.orange } },
      replace = { a = { fg = colors.purple, bg = colors.green } },
    }

    local empty = require('lualine.component'):extend()
    function empty:draw(default_highlight)
      self.status = ''
      self.applied_separator = ''
      self:apply_highlights(default_highlight)
      self:apply_section_separators()
      return self.status
    end

    -- Put proper separators and gaps between components in sections
    local function process_sections(sections)
      for name, section in pairs(sections) do
        local left = name:sub(9, 10) < 'x'
        for pos = 1, name ~= 'lualine_z' and #section or #section - 1 do
          table.insert(section, pos * 2, { empty, color = { fg = colors.blue, bg = colors.purple } })
        end
        for id, comp in ipairs(section) do
          if type(comp) ~= 'table' then
            comp = { comp }
            section[id] = comp
          end
          comp.separator = left and { right = '' } or { left = '' }
        end
      end
      return sections
    end

    local function search_result()
      if vim.v.hlsearch == 0 then
        return ''
      end
      local last_search = vim.fn.getreg '/'
      if not last_search or last_search == '' then
        return ''
      end
      local searchcount = vim.fn.searchcount { maxcount = 9999 }
      return last_search .. '(' .. searchcount.current .. '/' .. searchcount.total .. ')'
    end

    local function modified()
      if vim.bo.modified then
        return '+'
      elseif vim.bo.modifiable == false or vim.bo.readonly == true then
        return '-'
      end
      return ''
    end
    local icons = require 'custom.icons'

    require('lualine').setup {
      options = {
        theme = theme,
        component_separators = { left = '', right = '' },
        section_separators = { left = '', right = '' },
      },
      sections = {
        lualine_a = {
          {
            'mode',
          },
        },
        lualine_b = {
          'branch',
          {
            'diff',
            symbols = {
              added = icons.git.LineAdded,
              modified = icons.git.LineModified,
              removed = icons.git.LineRemoved,
            },
          },
        },
        lualine_c = {
          {
            'filename',
            path = 0,
            symbols = {
              modified = icons.ui.FileModified,
              readonly = icons.ui.ReadOnly,
              unnamed = icons.ui.NewFile,
              newfile = icons.ui.NewFile,
            },
          },
        },
        lualine_x = {
          {
            color = { fg = '#D69758' },
          },
        },
        lualine_y = {
          {
            'diagnostics',
            symbols = {
              error = icons.diagnostics.BoldError,
              warn = icons.diagnostics.BoldWarning,
              info = icons.diagnostics.BoldInformation,
              hint = icons.diagnostics.BoldHint,
            },
            colored = true,
            update_in_insert = false,
            always_visible = false,
          },
          { 'filetype', colored = false, icon_only = true },
        },
        lualine_z = { 'location' },
      },
      extensions = { 'quickfix', 'man', 'fugitive' },
    }
  end,
}
