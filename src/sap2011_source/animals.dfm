object animals_form: Tanimals_form
  Left = 0
  Top = 1
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Animals, SAP2011'
  ClientHeight = 618
  ClientWidth = 1069
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object datapages: TPageControl
    Left = 0
    Top = 0
    Width = 1065
    Height = 569
    ActivePage = new_animal
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    object select_animal: TTabSheet
      Caption = 'Select Animal'
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object messages: TLabel
        Left = 518
        Top = 60
        Width = 532
        Height = 19
        Caption = 
          'Navigate with arrow && up/down keys. Click field names to sort. ' +
          'Use search after sorting'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'Times New Roman'
        Font.Style = []
        ParentFont = False
      end
      object Label1: TLabel
        Left = 176
        Top = 22
        Width = 729
        Height = 21
        Caption = 
          'or click on the bird you want to select or use fillter && search' +
          ' fields to find it. Click Ok when done.'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -19
        Font.Name = 'Times New Roman'
        Font.Style = []
        ParentFont = False
      end
      object birds_navigator: TDBNavigator
        Left = 304
        Top = 57
        Width = 205
        Height = 33
        DataSource = DataSource1
        VisibleButtons = [nbFirst, nbLast, nbDelete, nbCancel, nbRefresh]
        TabOrder = 0
      end
      object animals_grid: TCRDBGrid
        Left = 0
        Top = 104
        Width = 1054
        Height = 427
        OptionsEx = [dgeEnableSort, dgeLocalFilter, dgeRecordCount, dgeSearchBar, dgeSummary]
        Color = clWhite
        DataSource = DataSource1
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        Options = [dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit]
        ParentFont = False
        TabOrder = 1
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = clMaroon
        TitleFont.Height = -13
        TitleFont.Name = 'MS Sans Serif'
        TitleFont.Style = [fsBold]
        OnCellClick = animals_gridCellClick
        OnDrawColumnCell = animals_gridDrawColumnCell
        OnTitleClick = animals_gridTitleClick
        Columns = <
          item
            Alignment = taCenter
            Expanded = False
            FieldName = 'bird_name'
            Width = 80
            Visible = True
          end
          item
            Alignment = taCenter
            Expanded = False
            FieldName = 'sex'
            Width = 35
            Visible = True
          end
          item
            Alignment = taCenter
            Expanded = False
            FieldName = 'hatched'
            Title.Alignment = taCenter
            Width = 75
            Visible = True
          end
          item
            Alignment = taCenter
            Expanded = False
            FieldName = 'weight'
            Title.Alignment = taCenter
            Width = 52
            Visible = True
          end
          item
            Alignment = taCenter
            Expanded = False
            FieldName = 'location'
            Title.Alignment = taCenter
            Width = 62
            Visible = True
          end
          item
            Alignment = taCenter
            Expanded = False
            FieldName = 'training'
            Title.Alignment = taCenter
            Width = 88
            Visible = True
          end
          item
            Alignment = taCenter
            Expanded = False
            FieldName = 'model'
            Title.Alignment = taCenter
            Width = 70
            Visible = True
          end
          item
            Alignment = taCenter
            Expanded = False
            FieldName = 'quota'
            Title.Alignment = taCenter
            Width = 50
            Visible = True
          end
          item
            Alignment = taCenter
            Expanded = False
            FieldName = 'audio'
            Title.Alignment = taCenter
            Width = 70
            Visible = True
          end
          item
            Alignment = taCenter
            Expanded = False
            FieldName = 'attributes'
            Title.Alignment = taCenter
            Width = 89
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'photoperiod'
            Title.Alignment = taCenter
            Width = 110
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'details'
            Width = 565
            Visible = True
          end>
      end
      object filter: TBitBtn
        Left = 72
        Top = 57
        Width = 113
        Height = 33
        Caption = 'Apply filter'
        DoubleBuffered = True
        Font.Charset = ANSI_CHARSET
        Font.Color = clNavy
        Font.Height = -15
        Font.Name = 'Comic Sans MS'
        Font.Style = [fsBold]
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          04000000000000010000120B0000120B00001000000000000000000000000000
          800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00FFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00FFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFF00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00FFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFF00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00FFFF
          FFFFFFFFFFFFFFFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFFFFFFFFF000F00FF
          FFFFFFFFFFFFFFFFFFFFFFF0000FFF0FFFFFFFFFFFFFFFFFFFFFFF00000FFF00
          FFFFFFFFFFFFFFFFFFFFF000000FFFF00FFFFFFFFFFFFFFFFFFF000000000000
          00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
        NumGlyphs = 2
        ParentDoubleBuffered = False
        ParentFont = False
        TabOrder = 2
        OnClick = filterClick
      end
      object clear_filters: TBitBtn
        Left = 184
        Top = 57
        Width = 121
        Height = 33
        Caption = 'Clear filters'
        DoubleBuffered = True
        Font.Charset = ANSI_CHARSET
        Font.Color = clNavy
        Font.Height = -15
        Font.Name = 'Comic Sans MS'
        Font.Style = [fsBold]
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          04000000000000010000120B0000120B00001000000000000000000000000000
          800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00FFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00FFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFF00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00FFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFF00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00FFFF
          FFFFFFFFFFFFFFFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFFFFFFFFF00FF00FF
          FFFFFFFFFFFFFFFFFFFFFFF0FFFFFF0FFFFFFFFFFFFFFFFFFFFFFF0FFFFFFFF0
          FFFFFFFFFFFFFFFFFFFFF0FFFFFFFFFF0FFFFFFFFFFFFFFFFFFF000000000000
          00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
        NumGlyphs = 2
        ParentDoubleBuffered = False
        ParentFont = False
        TabOrder = 3
        OnClick = clear_filtersClick
      end
      object BitBtn32: TBitBtn
        Left = 8
        Top = 57
        Width = 65
        Height = 33
        Caption = 'Help'
        DoubleBuffered = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -15
        Font.Name = 'Comic Sans MS'
        Font.Style = [fsBold]
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          04000000000000010000120B0000120B00001000000000000000000000000000
          800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
          3333333333FFFFF3333333333F797F3333333333F737373FF333333BFB999BFB
          33333337737773773F3333BFBF797FBFB33333733337333373F33BFBFBFBFBFB
          FB3337F33333F33337F33FBFBFB9BFBFBF3337333337F333373FFBFBFBF97BFB
          FBF37F333337FF33337FBFBFBFB99FBFBFB37F3333377FF3337FFBFBFBFB99FB
          FBF37F33333377FF337FBFBF77BF799FBFB37F333FF3377F337FFBFB99FB799B
          FBF373F377F3377F33733FBF997F799FBF3337F377FFF77337F33BFBF99999FB
          FB33373F37777733373333BFBF999FBFB3333373FF77733F7333333BFBFBFBFB
          3333333773FFFF77333333333FBFBF3333333333377777333333}
        NumGlyphs = 2
        ParentDoubleBuffered = False
        ParentFont = False
        TabOrder = 4
      end
      object BitBtn1: TBitBtn
        Left = 8
        Top = 16
        Width = 162
        Height = 35
        Caption = 'new animal'
        DoubleBuffered = True
        ParentDoubleBuffered = False
        TabOrder = 5
        OnClick = BitBtn1Click
      end
    end
    object new_animal: TTabSheet
      Caption = 'New Animal'
      ImageIndex = 1
      object Panel5: TPanel
        Left = 488
        Top = 258
        Width = 465
        Height = 79
        BevelOuter = bvLowered
        TabOrder = 0
        object Label11: TLabel
          Left = 118
          Top = 35
          Width = 35
          Height = 24
          Caption = 'Age'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -19
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object procedure1_age: TNumLab
          Left = 157
          Top = 40
          Width = 60
          Height = 20
          Alignment = taCenter
          ColorLabBakG = clSilver
          ColorLabText = clBlack
          ColorOutBakG = clSilver
          ColorOutText = clBlack
          ColorScheme = csBWG
          Comma = False
          DisplayType = dtFixP
          DTFormat = 'mmm-dd, yyyy'
          Empty = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ForcePlusSign = False
          FrameStyle = fsLowered
          LabelWidth = 58
          LeftSpace = 0
          LeftTextAlignment = taRightJustify
          OverflowIndicator = '*********'
          ParentFont = False
          Precision = 0
          RightTextAlignment = taLeftJustify
          StateText = 'undefined'
          Transparent = False
        end
        object Label12: TLabel
          Left = 65
          Top = 3
          Width = 104
          Height = 24
          Caption = 'Procedure 1'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -19
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object procedure1: TEdit
          Left = 12
          Top = 35
          Width = 93
          Height = 28
          Color = clWhite
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
          Text = 'isolation'
        end
        object procedure1_time: TDateTimePicker
          Left = 224
          Top = 40
          Width = 129
          Height = 28
          Date = 39098.000000000000000000
          Time = 39098.000000000000000000
          Kind = dtkTime
          TabOrder = 1
        end
        object set_time3: TBitBtn
          Left = 359
          Top = 24
          Width = 80
          Height = 33
          Caption = 'Now'
          DoubleBuffered = True
          Font.Charset = ANSI_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'Comic Sans MS'
          Font.Style = [fsBold]
          Glyph.Data = {
            76010000424D7601000000000000760000002800000020000000100000000100
            04000000000000010000120B0000120B00001000000000000000000000000000
            800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
            FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
            3333333333FFFFF3333333333000003333333333F77777FFF333333009999900
            3333333777777777FF33330998FFF899033333777333F3777FF33099FFFCFFF9
            903337773337333777F3309FFFFFFFCF9033377333F3337377FF098FF0FFFFFF
            890377F3373F3333377F09FFFF0FFFFFF90377F3F373FFFFF77F09FCFFF90000
            F90377F733377777377F09FFFFFFFFFFF90377F333333333377F098FFFFFFFFF
            890377FF3F33333F3773309FCFFFFFCF9033377F7333F37377F33099FFFCFFF9
            90333777FF37F3377733330998FCF899033333777FF7FF777333333009999900
            3333333777777777333333333000003333333333377777333333}
          NumGlyphs = 2
          ParentDoubleBuffered = False
          ParentFont = False
          TabOrder = 2
          OnClick = set_time3Click
        end
      end
      object Panel4: TPanel
        Left = 16
        Top = 370
        Width = 449
        Height = 79
        BevelOuter = bvLowered
        TabOrder = 1
        object Label9: TLabel
          Left = 7
          Top = 43
          Width = 106
          Height = 24
          Caption = 'Training age'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -19
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label10: TLabel
          Left = 9
          Top = 3
          Width = 69
          Height = 24
          Caption = 'Training'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -19
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object train_time: TDateTimePicker
          Left = 200
          Top = 48
          Width = 129
          Height = 28
          Date = 39098.000000000000000000
          Time = 39098.000000000000000000
          Kind = dtkTime
          TabOrder = 0
        end
        object set_time2: TBitBtn
          Left = 351
          Top = 32
          Width = 80
          Height = 33
          Caption = 'Now'
          DoubleBuffered = True
          Font.Charset = ANSI_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'Comic Sans MS'
          Font.Style = [fsBold]
          Glyph.Data = {
            76010000424D7601000000000000760000002800000020000000100000000100
            04000000000000010000120B0000120B00001000000000000000000000000000
            800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
            FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
            3333333333FFFFF3333333333000003333333333F77777FFF333333009999900
            3333333777777777FF33330998FFF899033333777333F3777FF33099FFFCFFF9
            903337773337333777F3309FFFFFFFCF9033377333F3337377FF098FF0FFFFFF
            890377F3373F3333377F09FFFF0FFFFFF90377F3F373FFFFF77F09FCFFF90000
            F90377F733377777377F09FFFFFFFFFFF90377F333333333377F098FFFFFFFFF
            890377FF3F33333F3773309FCFFFFFCF9033377F7333F37377F33099FFFCFFF9
            90333777FF37F3377733330998FCF899033333777FF7FF777333333009999900
            3333333777777777333333333000003333333333377777333333}
          NumGlyphs = 2
          ParentDoubleBuffered = False
          ParentFont = False
          TabOrder = 1
          OnClick = set_time2Click
        end
        object training: TEdit
          Left = 120
          Top = 43
          Width = 65
          Height = 28
          Color = clWhite
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 2
          OnChange = trainingChange
        end
      end
      object Panel3: TPanel
        Left = 16
        Top = 258
        Width = 457
        Height = 79
        BevelOuter = bvLowered
        TabOrder = 2
        object Label8: TLabel
          Left = 30
          Top = 19
          Width = 91
          Height = 24
          Caption = 'Age (days)'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -19
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object age: TNumLab
          Left = 31
          Top = 48
          Width = 60
          Height = 20
          Alignment = taCenter
          ColorLabBakG = clSilver
          ColorLabText = clBlack
          ColorOutBakG = clSilver
          ColorOutText = clBlack
          ColorScheme = csBWG
          Comma = False
          DisplayType = dtFixP
          DTFormat = 'mmm-dd, yyyy'
          Empty = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ForcePlusSign = False
          FrameStyle = fsLowered
          LabelWidth = 58
          LeftSpace = 0
          LeftTextAlignment = taRightJustify
          OverflowIndicator = '*********'
          ParentFont = False
          Precision = 0
          RightTextAlignment = taLeftJustify
          StateText = 'undefined'
          Transparent = False
        end
        object Label7: TLabel
          Left = 142
          Top = 43
          Width = 49
          Height = 24
          Caption = 'Hatch'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -19
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object set_time1: TBitBtn
          Left = 351
          Top = 40
          Width = 80
          Height = 33
          Caption = 'Now'
          DoubleBuffered = True
          Font.Charset = ANSI_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'Comic Sans MS'
          Font.Style = [fsBold]
          Glyph.Data = {
            76010000424D7601000000000000760000002800000020000000100000000100
            04000000000000010000120B0000120B00001000000000000000000000000000
            800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
            FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
            3333333333FFFFF3333333333000003333333333F77777FFF333333009999900
            3333333777777777FF33330998FFF899033333777333F3777FF33099FFFCFFF9
            903337773337333777F3309FFFFFFFCF9033377333F3337377FF098FF0FFFFFF
            890377F3373F3333377F09FFFF0FFFFFF90377F3F373FFFFF77F09FCFFF90000
            F90377F733377777377F09FFFFFFFFFFF90377F333333333377F098FFFFFFFFF
            890377FF3F33333F3773309FCFFFFFCF9033377F7333F37377F33099FFFCFFF9
            90333777FF37F3377733330998FCF899033333777FF7FF777333333009999900
            3333333777777777333333333000003333333333377777333333}
          NumGlyphs = 2
          ParentDoubleBuffered = False
          ParentFont = False
          TabOrder = 0
          OnClick = set_time1Click
        end
      end
      object bird_comments: TEdit
        Left = 88
        Top = 201
        Width = 857
        Height = 28
        Color = clWhite
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 3
      end
      object StaticText39: TStaticText
        Left = 8
        Top = 136
        Width = 37
        Height = 28
        Caption = 'Sex'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
      end
      object gender: TComboBox
        Left = 72
        Top = 136
        Width = 97
        Height = 28
        Color = clWhite
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 5
        Text = 'male'
        Items.Strings = (
          'male'
          'female')
      end
      object cage: TEdit
        Left = 256
        Top = 136
        Width = 98
        Height = 28
        Color = clWhite
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 6
      end
      object StaticText33: TStaticText
        Left = 178
        Top = 136
        Width = 75
        Height = 28
        Caption = 'Location'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 7
      end
      object StaticText38: TStaticText
        Left = 372
        Top = 136
        Width = 55
        Height = 28
        Caption = 'Quota'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 8
      end
      object song_quota: TEdit
        Left = 440
        Top = 136
        Width = 90
        Height = 28
        Color = clWhite
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 9
      end
      object StaticText35: TStaticText
        Left = 548
        Top = 136
        Width = 81
        Height = 28
        Caption = 'Attributes'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 10
      end
      object attributes: TEdit
        Left = 640
        Top = 136
        Width = 153
        Height = 28
        Color = clWhite
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 11
      end
      object photoperiod: TEdit
        Left = 816
        Top = 136
        Width = 129
        Height = 28
        Color = clWhite
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 12
      end
      object StaticText41: TStaticText
        Left = 820
        Top = 108
        Width = 107
        Height = 28
        Caption = 'Photoperiod'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 13
      end
      object audio: TEdit
        Left = 640
        Top = 63
        Width = 153
        Height = 28
        Color = clWhite
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 14
      end
      object StaticText34: TStaticText
        Left = 548
        Top = 63
        Width = 54
        Height = 28
        Caption = 'Audio'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 15
      end
      object song: TEdit
        Left = 432
        Top = 63
        Width = 98
        Height = 28
        Color = clWhite
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 16
      end
      object StaticText22: TStaticText
        Left = 364
        Top = 63
        Width = 57
        Height = 28
        Caption = 'Model'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 17
      end
      object weight: TEdit
        Left = 256
        Top = 63
        Width = 98
        Height = 28
        Color = clWhite
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 18
      end
      object StaticText23: TStaticText
        Left = 194
        Top = 63
        Width = 63
        Height = 28
        Caption = 'Weight'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 19
      end
      object name: TEdit
        Left = 72
        Top = 63
        Width = 98
        Height = 28
        Color = clWhite
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 20
        OnChange = nameChange
      end
      object StaticText21: TStaticText
        Left = 8
        Top = 63
        Width = 55
        Height = 28
        Caption = 'Name'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 21
      end
      object StaticText24: TStaticText
        Left = 17
        Top = 201
        Width = 59
        Height = 28
        Caption = 'Details'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 22
      end
      object Panel6: TPanel
        Left = 488
        Top = 370
        Width = 465
        Height = 79
        BevelOuter = bvLowered
        TabOrder = 23
        object Label13: TLabel
          Left = 73
          Top = 3
          Width = 104
          Height = 24
          Caption = 'Procedure 2'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -19
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label14: TLabel
          Left = 118
          Top = 35
          Width = 35
          Height = 24
          Caption = 'Age'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -19
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object procedure2_age: TNumLab
          Left = 157
          Top = 40
          Width = 60
          Height = 20
          Alignment = taCenter
          ColorLabBakG = clSilver
          ColorLabText = clBlack
          ColorOutBakG = clSilver
          ColorOutText = clBlack
          ColorScheme = csBWG
          Comma = False
          DisplayType = dtFixP
          DTFormat = 'mmm-dd, yyyy'
          Empty = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ForcePlusSign = False
          FrameStyle = fsLowered
          LabelWidth = 58
          LeftSpace = 0
          LeftTextAlignment = taRightJustify
          OverflowIndicator = '*********'
          ParentFont = False
          Precision = 0
          RightTextAlignment = taLeftJustify
          StateText = 'undefined'
          Transparent = False
        end
        object procedure2: TEdit
          Left = 12
          Top = 35
          Width = 85
          Height = 28
          Color = clWhite
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
        end
        object procedure2_time: TDateTimePicker
          Left = 224
          Top = 40
          Width = 129
          Height = 28
          Date = 39098.000000000000000000
          Time = 39098.000000000000000000
          Kind = dtkTime
          TabOrder = 1
        end
        object set_time4: TBitBtn
          Left = 367
          Top = 24
          Width = 80
          Height = 33
          Caption = 'Now'
          DoubleBuffered = True
          Font.Charset = ANSI_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'Comic Sans MS'
          Font.Style = [fsBold]
          Glyph.Data = {
            76010000424D7601000000000000760000002800000020000000100000000100
            04000000000000010000120B0000120B00001000000000000000000000000000
            800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
            FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
            3333333333FFFFF3333333333000003333333333F77777FFF333333009999900
            3333333777777777FF33330998FFF899033333777333F3777FF33099FFFCFFF9
            903337773337333777F3309FFFFFFFCF9033377333F3337377FF098FF0FFFFFF
            890377F3373F3333377F09FFFF0FFFFFF90377F3F373FFFFF77F09FCFFF90000
            F90377F733377777377F09FFFFFFFFFFF90377F333333333377F098FFFFFFFFF
            890377FF3F33333F3773309FCFFFFFCF9033377F7333F37377F33099FFFCFFF9
            90333777FF37F3377733330998FCF899033333777FF7FF777333333009999900
            3333333777777777333333333000003333333333377777333333}
          NumGlyphs = 2
          ParentDoubleBuffered = False
          ParentFont = False
          TabOrder = 2
          OnClick = set_time4Click
        end
      end
      object hatched: TAdvSmoothDatePicker
        Left = 253
        Top = 301
        Width = 108
        Height = 28
        Flat = False
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = []
        Lookup.Separator = ';'
        Color = clWindow
        Enabled = True
        TabOrder = 24
        Text = '2/4/2010'
        Visible = True
        Version = '1.5.4.1'
        ButtonStyle = bsDropDown
        ButtonWidth = 16
        Etched = False
        Glyph.Data = {
          DA020000424DDA0200000000000036000000280000000D0000000D0000000100
          200000000000A402000000000000000000000000000000000000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F00000000000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000000000000000000000000000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F0000000000000000000000000000000000000000000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F0000000000000000000000000000000
          0000000000000000000000000000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000}
        HideCalendarAfterSelection = True
        Calendar.Fill.Color = 15984090
        Calendar.Fill.ColorTo = 15785680
        Calendar.Fill.ColorMirror = clNone
        Calendar.Fill.ColorMirrorTo = clNone
        Calendar.Fill.Rounding = 0
        Calendar.Fill.ShadowOffset = 0
        Calendar.Animation = True
        Calendar.ShowCurrentDate = True
        Calendar.DateAppearance.DateFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.DateFont.Color = clWindowText
        Calendar.DateAppearance.DateFont.Height = -11
        Calendar.DateAppearance.DateFont.Name = 'Tahoma'
        Calendar.DateAppearance.DateFont.Style = []
        Calendar.DateAppearance.DateFill.Color = 16773091
        Calendar.DateAppearance.DateFill.ColorTo = 16768452
        Calendar.DateAppearance.DateFill.ColorMirror = 16765357
        Calendar.DateAppearance.DateFill.ColorMirrorTo = 16767936
        Calendar.DateAppearance.DateFill.GradientMirrorType = gtVertical
        Calendar.DateAppearance.DateFill.Rounding = 0
        Calendar.DateAppearance.DateFill.ShadowOffset = 0
        Calendar.DateAppearance.DayOfWeekFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.DayOfWeekFont.Color = clWindowText
        Calendar.DateAppearance.DayOfWeekFont.Height = -11
        Calendar.DateAppearance.DayOfWeekFont.Name = 'Tahoma'
        Calendar.DateAppearance.DayOfWeekFont.Style = []
        Calendar.DateAppearance.DayOfWeekFill.Color = 15984090
        Calendar.DateAppearance.DayOfWeekFill.ColorTo = 15785680
        Calendar.DateAppearance.DayOfWeekFill.ColorMirror = clNone
        Calendar.DateAppearance.DayOfWeekFill.ColorMirrorTo = clNone
        Calendar.DateAppearance.DayOfWeekFill.Rounding = 0
        Calendar.DateAppearance.DayOfWeekFill.ShadowOffset = 0
        Calendar.DateAppearance.SelectedDateFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.SelectedDateFont.Color = clWindowText
        Calendar.DateAppearance.SelectedDateFont.Height = -11
        Calendar.DateAppearance.SelectedDateFont.Name = 'Tahoma'
        Calendar.DateAppearance.SelectedDateFont.Style = []
        Calendar.DateAppearance.SelectedDateFill.Color = 11196927
        Calendar.DateAppearance.SelectedDateFill.ColorTo = 7257087
        Calendar.DateAppearance.SelectedDateFill.ColorMirror = 4370174
        Calendar.DateAppearance.SelectedDateFill.ColorMirrorTo = 8053246
        Calendar.DateAppearance.SelectedDateFill.GradientMirrorType = gtVertical
        Calendar.DateAppearance.SelectedDateFill.BorderColor = 4370174
        Calendar.DateAppearance.SelectedDateFill.Rounding = 0
        Calendar.DateAppearance.SelectedDateFill.ShadowOffset = 0
        Calendar.DateAppearance.CurrentDateFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.CurrentDateFont.Color = clWindowText
        Calendar.DateAppearance.CurrentDateFont.Height = -11
        Calendar.DateAppearance.CurrentDateFont.Name = 'Tahoma'
        Calendar.DateAppearance.CurrentDateFont.Style = []
        Calendar.DateAppearance.CurrentDateFill.Color = 7778289
        Calendar.DateAppearance.CurrentDateFill.ColorTo = 4296947
        Calendar.DateAppearance.CurrentDateFill.ColorMirror = 946929
        Calendar.DateAppearance.CurrentDateFill.ColorMirrorTo = 5021693
        Calendar.DateAppearance.CurrentDateFill.GradientMirrorType = gtVertical
        Calendar.DateAppearance.CurrentDateFill.BorderColor = 4548219
        Calendar.DateAppearance.CurrentDateFill.Rounding = 0
        Calendar.DateAppearance.CurrentDateFill.ShadowOffset = 0
        Calendar.DateAppearance.WeekendFill.Color = 16773091
        Calendar.DateAppearance.WeekendFill.ColorTo = 16768452
        Calendar.DateAppearance.WeekendFill.ColorMirror = 16765357
        Calendar.DateAppearance.WeekendFill.ColorMirrorTo = 16767936
        Calendar.DateAppearance.WeekendFill.GradientMirrorType = gtVertical
        Calendar.DateAppearance.WeekendFill.Rounding = 0
        Calendar.DateAppearance.WeekendFill.ShadowOffset = 0
        Calendar.DateAppearance.WeekendFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.WeekendFont.Color = clWindowText
        Calendar.DateAppearance.WeekendFont.Height = -11
        Calendar.DateAppearance.WeekendFont.Name = 'Tahoma'
        Calendar.DateAppearance.WeekendFont.Style = []
        Calendar.DateAppearance.HoverDateFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.HoverDateFont.Color = clWindowText
        Calendar.DateAppearance.HoverDateFont.Height = -11
        Calendar.DateAppearance.HoverDateFont.Name = 'Tahoma'
        Calendar.DateAppearance.HoverDateFont.Style = []
        Calendar.DateAppearance.HoverDateFill.Color = 15465983
        Calendar.DateAppearance.HoverDateFill.ColorTo = 11332863
        Calendar.DateAppearance.HoverDateFill.ColorMirror = 5888767
        Calendar.DateAppearance.HoverDateFill.ColorMirrorTo = 10807807
        Calendar.DateAppearance.HoverDateFill.GradientMirrorType = gtVertical
        Calendar.DateAppearance.HoverDateFill.BorderColor = 10079963
        Calendar.DateAppearance.HoverDateFill.Rounding = 0
        Calendar.DateAppearance.HoverDateFill.ShadowOffset = 0
        Calendar.DateAppearance.MonthDateFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.MonthDateFont.Color = clWindowText
        Calendar.DateAppearance.MonthDateFont.Height = -11
        Calendar.DateAppearance.MonthDateFont.Name = 'Tahoma'
        Calendar.DateAppearance.MonthDateFont.Style = []
        Calendar.DateAppearance.YearDateFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.YearDateFont.Color = clWindowText
        Calendar.DateAppearance.YearDateFont.Height = -11
        Calendar.DateAppearance.YearDateFont.Name = 'Tahoma'
        Calendar.DateAppearance.YearDateFont.Style = []
        Calendar.DateAppearance.WeekNumbers.Font.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.WeekNumbers.Font.Color = clWindowText
        Calendar.DateAppearance.WeekNumbers.Font.Height = -11
        Calendar.DateAppearance.WeekNumbers.Font.Name = 'Tahoma'
        Calendar.DateAppearance.WeekNumbers.Font.Style = []
        Calendar.DateAppearance.WeekNumbers.Fill.Color = 15984090
        Calendar.DateAppearance.WeekNumbers.Fill.ColorTo = 15785680
        Calendar.DateAppearance.WeekNumbers.Fill.ColorMirror = clNone
        Calendar.DateAppearance.WeekNumbers.Fill.ColorMirrorTo = clNone
        Calendar.DateAppearance.WeekNumbers.Fill.Rounding = 0
        Calendar.DateAppearance.WeekNumbers.Fill.ShadowOffset = 0
        Calendar.DateAppearance.DisabledDateFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.DisabledDateFont.Color = clGray
        Calendar.DateAppearance.DisabledDateFont.Height = -11
        Calendar.DateAppearance.DisabledDateFont.Name = 'Tahoma'
        Calendar.DateAppearance.DisabledDateFont.Style = []
        Calendar.DateAppearance.DisabledDateFill.Color = 15921906
        Calendar.DateAppearance.DisabledDateFill.ColorTo = 11974326
        Calendar.DateAppearance.DisabledDateFill.ColorMirror = 11974326
        Calendar.DateAppearance.DisabledDateFill.ColorMirrorTo = 15921906
        Calendar.DateAppearance.DisabledDateFill.GradientMirrorType = gtVertical
        Calendar.DateAppearance.DisabledDateFill.Rounding = 0
        Calendar.DateAppearance.DisabledDateFill.ShadowOffset = 0
        Calendar.DateAppearance.DateBeforeFill.Color = clNone
        Calendar.DateAppearance.DateBeforeFill.ColorMirror = clNone
        Calendar.DateAppearance.DateBeforeFill.ColorMirrorTo = clNone
        Calendar.DateAppearance.DateBeforeFill.Rounding = 0
        Calendar.DateAppearance.DateBeforeFill.ShadowOffset = 0
        Calendar.DateAppearance.DateAfterFill.Color = clNone
        Calendar.DateAppearance.DateAfterFill.ColorMirror = clNone
        Calendar.DateAppearance.DateAfterFill.ColorMirrorTo = clNone
        Calendar.DateAppearance.DateAfterFill.Rounding = 0
        Calendar.DateAppearance.DateAfterFill.ShadowOffset = 0
        Calendar.DateAppearance.DateBeforeFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.DateBeforeFont.Color = clSilver
        Calendar.DateAppearance.DateBeforeFont.Height = -11
        Calendar.DateAppearance.DateBeforeFont.Name = 'Tahoma'
        Calendar.DateAppearance.DateBeforeFont.Style = []
        Calendar.DateAppearance.DateAfterFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.DateAfterFont.Color = clSilver
        Calendar.DateAppearance.DateAfterFont.Height = -11
        Calendar.DateAppearance.DateAfterFont.Name = 'Tahoma'
        Calendar.DateAppearance.DateAfterFont.Style = []
        Calendar.StatusAppearance.Fill.Color = clRed
        Calendar.StatusAppearance.Fill.ColorMirror = clNone
        Calendar.StatusAppearance.Fill.ColorMirrorTo = clNone
        Calendar.StatusAppearance.Fill.GradientType = gtSolid
        Calendar.StatusAppearance.Fill.BorderColor = clGray
        Calendar.StatusAppearance.Fill.Rounding = 0
        Calendar.StatusAppearance.Fill.ShadowOffset = 0
        Calendar.StatusAppearance.Font.Charset = DEFAULT_CHARSET
        Calendar.StatusAppearance.Font.Color = clWhite
        Calendar.StatusAppearance.Font.Height = -11
        Calendar.StatusAppearance.Font.Name = 'Tahoma'
        Calendar.StatusAppearance.Font.Style = []
        Calendar.Footer.Fill.Color = 16773091
        Calendar.Footer.Fill.ColorTo = 16765615
        Calendar.Footer.Fill.ColorMirror = clNone
        Calendar.Footer.Fill.ColorMirrorTo = clNone
        Calendar.Footer.Fill.BorderColor = 16765615
        Calendar.Footer.Fill.Rounding = 0
        Calendar.Footer.Fill.ShadowOffset = 0
        Calendar.Footer.Font.Charset = DEFAULT_CHARSET
        Calendar.Footer.Font.Color = 7485192
        Calendar.Footer.Font.Height = -11
        Calendar.Footer.Font.Name = 'Tahoma'
        Calendar.Footer.Font.Style = []
        Calendar.Header.Fill.Color = 16773091
        Calendar.Header.Fill.ColorTo = 16765615
        Calendar.Header.Fill.ColorMirror = clNone
        Calendar.Header.Fill.ColorMirrorTo = clNone
        Calendar.Header.Fill.BorderColor = 16765615
        Calendar.Header.Fill.Rounding = 0
        Calendar.Header.Fill.ShadowOffset = 0
        Calendar.Header.ArrowColor = 7485192
        Calendar.Header.Font.Charset = DEFAULT_CHARSET
        Calendar.Header.Font.Color = 7485192
        Calendar.Header.Font.Height = -11
        Calendar.Header.Font.Name = 'Tahoma'
        Calendar.Header.Font.Style = []
        Calendar.Width = 257
        Calendar.Height = 249
        Calendar.ShowHint = False
        Date = 40213.000000000000000000
      end
      object trained: TAdvSmoothDatePicker
        Left = 216
        Top = 373
        Width = 108
        Height = 28
        Flat = False
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = []
        Lookup.Separator = ';'
        Color = clWindow
        Enabled = True
        TabOrder = 25
        Text = '2/4/2010'
        Visible = True
        Version = '1.5.4.1'
        ButtonStyle = bsDropDown
        ButtonWidth = 16
        Etched = False
        Glyph.Data = {
          DA020000424DDA0200000000000036000000280000000D0000000D0000000100
          200000000000A402000000000000000000000000000000000000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F00000000000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000000000000000000000000000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F0000000000000000000000000000000000000000000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F0000000000000000000000000000000
          0000000000000000000000000000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000}
        HideCalendarAfterSelection = True
        Calendar.Fill.Color = 15984090
        Calendar.Fill.ColorTo = 15785680
        Calendar.Fill.ColorMirror = clNone
        Calendar.Fill.ColorMirrorTo = clNone
        Calendar.Fill.Rounding = 0
        Calendar.Fill.ShadowOffset = 0
        Calendar.Animation = True
        Calendar.ShowCurrentDate = True
        Calendar.DateAppearance.DateFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.DateFont.Color = clWindowText
        Calendar.DateAppearance.DateFont.Height = -11
        Calendar.DateAppearance.DateFont.Name = 'Tahoma'
        Calendar.DateAppearance.DateFont.Style = []
        Calendar.DateAppearance.DateFill.Color = 16773091
        Calendar.DateAppearance.DateFill.ColorTo = 16768452
        Calendar.DateAppearance.DateFill.ColorMirror = 16765357
        Calendar.DateAppearance.DateFill.ColorMirrorTo = 16767936
        Calendar.DateAppearance.DateFill.GradientMirrorType = gtVertical
        Calendar.DateAppearance.DateFill.Rounding = 0
        Calendar.DateAppearance.DateFill.ShadowOffset = 0
        Calendar.DateAppearance.DayOfWeekFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.DayOfWeekFont.Color = clWindowText
        Calendar.DateAppearance.DayOfWeekFont.Height = -11
        Calendar.DateAppearance.DayOfWeekFont.Name = 'Tahoma'
        Calendar.DateAppearance.DayOfWeekFont.Style = []
        Calendar.DateAppearance.DayOfWeekFill.Color = 15984090
        Calendar.DateAppearance.DayOfWeekFill.ColorTo = 15785680
        Calendar.DateAppearance.DayOfWeekFill.ColorMirror = clNone
        Calendar.DateAppearance.DayOfWeekFill.ColorMirrorTo = clNone
        Calendar.DateAppearance.DayOfWeekFill.Rounding = 0
        Calendar.DateAppearance.DayOfWeekFill.ShadowOffset = 0
        Calendar.DateAppearance.SelectedDateFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.SelectedDateFont.Color = clWindowText
        Calendar.DateAppearance.SelectedDateFont.Height = -11
        Calendar.DateAppearance.SelectedDateFont.Name = 'Tahoma'
        Calendar.DateAppearance.SelectedDateFont.Style = []
        Calendar.DateAppearance.SelectedDateFill.Color = 11196927
        Calendar.DateAppearance.SelectedDateFill.ColorTo = 7257087
        Calendar.DateAppearance.SelectedDateFill.ColorMirror = 4370174
        Calendar.DateAppearance.SelectedDateFill.ColorMirrorTo = 8053246
        Calendar.DateAppearance.SelectedDateFill.GradientMirrorType = gtVertical
        Calendar.DateAppearance.SelectedDateFill.BorderColor = 4370174
        Calendar.DateAppearance.SelectedDateFill.Rounding = 0
        Calendar.DateAppearance.SelectedDateFill.ShadowOffset = 0
        Calendar.DateAppearance.CurrentDateFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.CurrentDateFont.Color = clWindowText
        Calendar.DateAppearance.CurrentDateFont.Height = -11
        Calendar.DateAppearance.CurrentDateFont.Name = 'Tahoma'
        Calendar.DateAppearance.CurrentDateFont.Style = []
        Calendar.DateAppearance.CurrentDateFill.Color = 7778289
        Calendar.DateAppearance.CurrentDateFill.ColorTo = 4296947
        Calendar.DateAppearance.CurrentDateFill.ColorMirror = 946929
        Calendar.DateAppearance.CurrentDateFill.ColorMirrorTo = 5021693
        Calendar.DateAppearance.CurrentDateFill.GradientMirrorType = gtVertical
        Calendar.DateAppearance.CurrentDateFill.BorderColor = 4548219
        Calendar.DateAppearance.CurrentDateFill.Rounding = 0
        Calendar.DateAppearance.CurrentDateFill.ShadowOffset = 0
        Calendar.DateAppearance.WeekendFill.Color = 16773091
        Calendar.DateAppearance.WeekendFill.ColorTo = 16768452
        Calendar.DateAppearance.WeekendFill.ColorMirror = 16765357
        Calendar.DateAppearance.WeekendFill.ColorMirrorTo = 16767936
        Calendar.DateAppearance.WeekendFill.GradientMirrorType = gtVertical
        Calendar.DateAppearance.WeekendFill.Rounding = 0
        Calendar.DateAppearance.WeekendFill.ShadowOffset = 0
        Calendar.DateAppearance.WeekendFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.WeekendFont.Color = clWindowText
        Calendar.DateAppearance.WeekendFont.Height = -11
        Calendar.DateAppearance.WeekendFont.Name = 'Tahoma'
        Calendar.DateAppearance.WeekendFont.Style = []
        Calendar.DateAppearance.HoverDateFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.HoverDateFont.Color = clWindowText
        Calendar.DateAppearance.HoverDateFont.Height = -11
        Calendar.DateAppearance.HoverDateFont.Name = 'Tahoma'
        Calendar.DateAppearance.HoverDateFont.Style = []
        Calendar.DateAppearance.HoverDateFill.Color = 15465983
        Calendar.DateAppearance.HoverDateFill.ColorTo = 11332863
        Calendar.DateAppearance.HoverDateFill.ColorMirror = 5888767
        Calendar.DateAppearance.HoverDateFill.ColorMirrorTo = 10807807
        Calendar.DateAppearance.HoverDateFill.GradientMirrorType = gtVertical
        Calendar.DateAppearance.HoverDateFill.BorderColor = 10079963
        Calendar.DateAppearance.HoverDateFill.Rounding = 0
        Calendar.DateAppearance.HoverDateFill.ShadowOffset = 0
        Calendar.DateAppearance.MonthDateFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.MonthDateFont.Color = clWindowText
        Calendar.DateAppearance.MonthDateFont.Height = -11
        Calendar.DateAppearance.MonthDateFont.Name = 'Tahoma'
        Calendar.DateAppearance.MonthDateFont.Style = []
        Calendar.DateAppearance.YearDateFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.YearDateFont.Color = clWindowText
        Calendar.DateAppearance.YearDateFont.Height = -11
        Calendar.DateAppearance.YearDateFont.Name = 'Tahoma'
        Calendar.DateAppearance.YearDateFont.Style = []
        Calendar.DateAppearance.WeekNumbers.Font.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.WeekNumbers.Font.Color = clWindowText
        Calendar.DateAppearance.WeekNumbers.Font.Height = -11
        Calendar.DateAppearance.WeekNumbers.Font.Name = 'Tahoma'
        Calendar.DateAppearance.WeekNumbers.Font.Style = []
        Calendar.DateAppearance.WeekNumbers.Fill.Color = 15984090
        Calendar.DateAppearance.WeekNumbers.Fill.ColorTo = 15785680
        Calendar.DateAppearance.WeekNumbers.Fill.ColorMirror = clNone
        Calendar.DateAppearance.WeekNumbers.Fill.ColorMirrorTo = clNone
        Calendar.DateAppearance.WeekNumbers.Fill.Rounding = 0
        Calendar.DateAppearance.WeekNumbers.Fill.ShadowOffset = 0
        Calendar.DateAppearance.DisabledDateFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.DisabledDateFont.Color = clGray
        Calendar.DateAppearance.DisabledDateFont.Height = -11
        Calendar.DateAppearance.DisabledDateFont.Name = 'Tahoma'
        Calendar.DateAppearance.DisabledDateFont.Style = []
        Calendar.DateAppearance.DisabledDateFill.Color = 15921906
        Calendar.DateAppearance.DisabledDateFill.ColorTo = 11974326
        Calendar.DateAppearance.DisabledDateFill.ColorMirror = 11974326
        Calendar.DateAppearance.DisabledDateFill.ColorMirrorTo = 15921906
        Calendar.DateAppearance.DisabledDateFill.GradientMirrorType = gtVertical
        Calendar.DateAppearance.DisabledDateFill.Rounding = 0
        Calendar.DateAppearance.DisabledDateFill.ShadowOffset = 0
        Calendar.DateAppearance.DateBeforeFill.Color = clNone
        Calendar.DateAppearance.DateBeforeFill.ColorMirror = clNone
        Calendar.DateAppearance.DateBeforeFill.ColorMirrorTo = clNone
        Calendar.DateAppearance.DateBeforeFill.Rounding = 0
        Calendar.DateAppearance.DateBeforeFill.ShadowOffset = 0
        Calendar.DateAppearance.DateAfterFill.Color = clNone
        Calendar.DateAppearance.DateAfterFill.ColorMirror = clNone
        Calendar.DateAppearance.DateAfterFill.ColorMirrorTo = clNone
        Calendar.DateAppearance.DateAfterFill.Rounding = 0
        Calendar.DateAppearance.DateAfterFill.ShadowOffset = 0
        Calendar.DateAppearance.DateBeforeFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.DateBeforeFont.Color = clSilver
        Calendar.DateAppearance.DateBeforeFont.Height = -11
        Calendar.DateAppearance.DateBeforeFont.Name = 'Tahoma'
        Calendar.DateAppearance.DateBeforeFont.Style = []
        Calendar.DateAppearance.DateAfterFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.DateAfterFont.Color = clSilver
        Calendar.DateAppearance.DateAfterFont.Height = -11
        Calendar.DateAppearance.DateAfterFont.Name = 'Tahoma'
        Calendar.DateAppearance.DateAfterFont.Style = []
        Calendar.StatusAppearance.Fill.Color = clRed
        Calendar.StatusAppearance.Fill.ColorMirror = clNone
        Calendar.StatusAppearance.Fill.ColorMirrorTo = clNone
        Calendar.StatusAppearance.Fill.GradientType = gtSolid
        Calendar.StatusAppearance.Fill.BorderColor = clGray
        Calendar.StatusAppearance.Fill.Rounding = 0
        Calendar.StatusAppearance.Fill.ShadowOffset = 0
        Calendar.StatusAppearance.Font.Charset = DEFAULT_CHARSET
        Calendar.StatusAppearance.Font.Color = clWhite
        Calendar.StatusAppearance.Font.Height = -11
        Calendar.StatusAppearance.Font.Name = 'Tahoma'
        Calendar.StatusAppearance.Font.Style = []
        Calendar.Footer.Fill.Color = 16773091
        Calendar.Footer.Fill.ColorTo = 16765615
        Calendar.Footer.Fill.ColorMirror = clNone
        Calendar.Footer.Fill.ColorMirrorTo = clNone
        Calendar.Footer.Fill.BorderColor = 16765615
        Calendar.Footer.Fill.Rounding = 0
        Calendar.Footer.Fill.ShadowOffset = 0
        Calendar.Footer.Font.Charset = DEFAULT_CHARSET
        Calendar.Footer.Font.Color = 7485192
        Calendar.Footer.Font.Height = -11
        Calendar.Footer.Font.Name = 'Tahoma'
        Calendar.Footer.Font.Style = []
        Calendar.Header.Fill.Color = 16773091
        Calendar.Header.Fill.ColorTo = 16765615
        Calendar.Header.Fill.ColorMirror = clNone
        Calendar.Header.Fill.ColorMirrorTo = clNone
        Calendar.Header.Fill.BorderColor = 16765615
        Calendar.Header.Fill.Rounding = 0
        Calendar.Header.Fill.ShadowOffset = 0
        Calendar.Header.ArrowColor = 7485192
        Calendar.Header.Font.Charset = DEFAULT_CHARSET
        Calendar.Header.Font.Color = 7485192
        Calendar.Header.Font.Height = -11
        Calendar.Header.Font.Name = 'Tahoma'
        Calendar.Header.Font.Style = []
        Calendar.Width = 257
        Calendar.Height = 249
        Calendar.ShowHint = False
        Date = 40213.000000000000000000
      end
      object procedure1_schedule: TAdvSmoothDatePicker
        Left = 711
        Top = 268
        Width = 108
        Height = 28
        Flat = False
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = []
        Lookup.Separator = ';'
        Color = clWindow
        Enabled = True
        TabOrder = 26
        Text = '2/4/2010'
        Visible = True
        Version = '1.5.4.1'
        ButtonStyle = bsDropDown
        ButtonWidth = 16
        Etched = False
        Glyph.Data = {
          DA020000424DDA0200000000000036000000280000000D0000000D0000000100
          200000000000A402000000000000000000000000000000000000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F00000000000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000000000000000000000000000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F0000000000000000000000000000000000000000000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F0000000000000000000000000000000
          0000000000000000000000000000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000}
        HideCalendarAfterSelection = True
        Calendar.Fill.Color = 15984090
        Calendar.Fill.ColorTo = 15785680
        Calendar.Fill.ColorMirror = clNone
        Calendar.Fill.ColorMirrorTo = clNone
        Calendar.Fill.Rounding = 0
        Calendar.Fill.ShadowOffset = 0
        Calendar.Animation = True
        Calendar.ShowCurrentDate = True
        Calendar.DateAppearance.DateFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.DateFont.Color = clWindowText
        Calendar.DateAppearance.DateFont.Height = -11
        Calendar.DateAppearance.DateFont.Name = 'Tahoma'
        Calendar.DateAppearance.DateFont.Style = []
        Calendar.DateAppearance.DateFill.Color = 16773091
        Calendar.DateAppearance.DateFill.ColorTo = 16768452
        Calendar.DateAppearance.DateFill.ColorMirror = 16765357
        Calendar.DateAppearance.DateFill.ColorMirrorTo = 16767936
        Calendar.DateAppearance.DateFill.GradientMirrorType = gtVertical
        Calendar.DateAppearance.DateFill.Rounding = 0
        Calendar.DateAppearance.DateFill.ShadowOffset = 0
        Calendar.DateAppearance.DayOfWeekFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.DayOfWeekFont.Color = clWindowText
        Calendar.DateAppearance.DayOfWeekFont.Height = -11
        Calendar.DateAppearance.DayOfWeekFont.Name = 'Tahoma'
        Calendar.DateAppearance.DayOfWeekFont.Style = []
        Calendar.DateAppearance.DayOfWeekFill.Color = 15984090
        Calendar.DateAppearance.DayOfWeekFill.ColorTo = 15785680
        Calendar.DateAppearance.DayOfWeekFill.ColorMirror = clNone
        Calendar.DateAppearance.DayOfWeekFill.ColorMirrorTo = clNone
        Calendar.DateAppearance.DayOfWeekFill.Rounding = 0
        Calendar.DateAppearance.DayOfWeekFill.ShadowOffset = 0
        Calendar.DateAppearance.SelectedDateFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.SelectedDateFont.Color = clWindowText
        Calendar.DateAppearance.SelectedDateFont.Height = -11
        Calendar.DateAppearance.SelectedDateFont.Name = 'Tahoma'
        Calendar.DateAppearance.SelectedDateFont.Style = []
        Calendar.DateAppearance.SelectedDateFill.Color = 11196927
        Calendar.DateAppearance.SelectedDateFill.ColorTo = 7257087
        Calendar.DateAppearance.SelectedDateFill.ColorMirror = 4370174
        Calendar.DateAppearance.SelectedDateFill.ColorMirrorTo = 8053246
        Calendar.DateAppearance.SelectedDateFill.GradientMirrorType = gtVertical
        Calendar.DateAppearance.SelectedDateFill.BorderColor = 4370174
        Calendar.DateAppearance.SelectedDateFill.Rounding = 0
        Calendar.DateAppearance.SelectedDateFill.ShadowOffset = 0
        Calendar.DateAppearance.CurrentDateFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.CurrentDateFont.Color = clWindowText
        Calendar.DateAppearance.CurrentDateFont.Height = -11
        Calendar.DateAppearance.CurrentDateFont.Name = 'Tahoma'
        Calendar.DateAppearance.CurrentDateFont.Style = []
        Calendar.DateAppearance.CurrentDateFill.Color = 7778289
        Calendar.DateAppearance.CurrentDateFill.ColorTo = 4296947
        Calendar.DateAppearance.CurrentDateFill.ColorMirror = 946929
        Calendar.DateAppearance.CurrentDateFill.ColorMirrorTo = 5021693
        Calendar.DateAppearance.CurrentDateFill.GradientMirrorType = gtVertical
        Calendar.DateAppearance.CurrentDateFill.BorderColor = 4548219
        Calendar.DateAppearance.CurrentDateFill.Rounding = 0
        Calendar.DateAppearance.CurrentDateFill.ShadowOffset = 0
        Calendar.DateAppearance.WeekendFill.Color = 16773091
        Calendar.DateAppearance.WeekendFill.ColorTo = 16768452
        Calendar.DateAppearance.WeekendFill.ColorMirror = 16765357
        Calendar.DateAppearance.WeekendFill.ColorMirrorTo = 16767936
        Calendar.DateAppearance.WeekendFill.GradientMirrorType = gtVertical
        Calendar.DateAppearance.WeekendFill.Rounding = 0
        Calendar.DateAppearance.WeekendFill.ShadowOffset = 0
        Calendar.DateAppearance.WeekendFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.WeekendFont.Color = clWindowText
        Calendar.DateAppearance.WeekendFont.Height = -11
        Calendar.DateAppearance.WeekendFont.Name = 'Tahoma'
        Calendar.DateAppearance.WeekendFont.Style = []
        Calendar.DateAppearance.HoverDateFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.HoverDateFont.Color = clWindowText
        Calendar.DateAppearance.HoverDateFont.Height = -11
        Calendar.DateAppearance.HoverDateFont.Name = 'Tahoma'
        Calendar.DateAppearance.HoverDateFont.Style = []
        Calendar.DateAppearance.HoverDateFill.Color = 15465983
        Calendar.DateAppearance.HoverDateFill.ColorTo = 11332863
        Calendar.DateAppearance.HoverDateFill.ColorMirror = 5888767
        Calendar.DateAppearance.HoverDateFill.ColorMirrorTo = 10807807
        Calendar.DateAppearance.HoverDateFill.GradientMirrorType = gtVertical
        Calendar.DateAppearance.HoverDateFill.BorderColor = 10079963
        Calendar.DateAppearance.HoverDateFill.Rounding = 0
        Calendar.DateAppearance.HoverDateFill.ShadowOffset = 0
        Calendar.DateAppearance.MonthDateFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.MonthDateFont.Color = clWindowText
        Calendar.DateAppearance.MonthDateFont.Height = -11
        Calendar.DateAppearance.MonthDateFont.Name = 'Tahoma'
        Calendar.DateAppearance.MonthDateFont.Style = []
        Calendar.DateAppearance.YearDateFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.YearDateFont.Color = clWindowText
        Calendar.DateAppearance.YearDateFont.Height = -11
        Calendar.DateAppearance.YearDateFont.Name = 'Tahoma'
        Calendar.DateAppearance.YearDateFont.Style = []
        Calendar.DateAppearance.WeekNumbers.Font.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.WeekNumbers.Font.Color = clWindowText
        Calendar.DateAppearance.WeekNumbers.Font.Height = -11
        Calendar.DateAppearance.WeekNumbers.Font.Name = 'Tahoma'
        Calendar.DateAppearance.WeekNumbers.Font.Style = []
        Calendar.DateAppearance.WeekNumbers.Fill.Color = 15984090
        Calendar.DateAppearance.WeekNumbers.Fill.ColorTo = 15785680
        Calendar.DateAppearance.WeekNumbers.Fill.ColorMirror = clNone
        Calendar.DateAppearance.WeekNumbers.Fill.ColorMirrorTo = clNone
        Calendar.DateAppearance.WeekNumbers.Fill.Rounding = 0
        Calendar.DateAppearance.WeekNumbers.Fill.ShadowOffset = 0
        Calendar.DateAppearance.DisabledDateFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.DisabledDateFont.Color = clGray
        Calendar.DateAppearance.DisabledDateFont.Height = -11
        Calendar.DateAppearance.DisabledDateFont.Name = 'Tahoma'
        Calendar.DateAppearance.DisabledDateFont.Style = []
        Calendar.DateAppearance.DisabledDateFill.Color = 15921906
        Calendar.DateAppearance.DisabledDateFill.ColorTo = 11974326
        Calendar.DateAppearance.DisabledDateFill.ColorMirror = 11974326
        Calendar.DateAppearance.DisabledDateFill.ColorMirrorTo = 15921906
        Calendar.DateAppearance.DisabledDateFill.GradientMirrorType = gtVertical
        Calendar.DateAppearance.DisabledDateFill.Rounding = 0
        Calendar.DateAppearance.DisabledDateFill.ShadowOffset = 0
        Calendar.DateAppearance.DateBeforeFill.Color = clNone
        Calendar.DateAppearance.DateBeforeFill.ColorMirror = clNone
        Calendar.DateAppearance.DateBeforeFill.ColorMirrorTo = clNone
        Calendar.DateAppearance.DateBeforeFill.Rounding = 0
        Calendar.DateAppearance.DateBeforeFill.ShadowOffset = 0
        Calendar.DateAppearance.DateAfterFill.Color = clNone
        Calendar.DateAppearance.DateAfterFill.ColorMirror = clNone
        Calendar.DateAppearance.DateAfterFill.ColorMirrorTo = clNone
        Calendar.DateAppearance.DateAfterFill.Rounding = 0
        Calendar.DateAppearance.DateAfterFill.ShadowOffset = 0
        Calendar.DateAppearance.DateBeforeFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.DateBeforeFont.Color = clSilver
        Calendar.DateAppearance.DateBeforeFont.Height = -11
        Calendar.DateAppearance.DateBeforeFont.Name = 'Tahoma'
        Calendar.DateAppearance.DateBeforeFont.Style = []
        Calendar.DateAppearance.DateAfterFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.DateAfterFont.Color = clSilver
        Calendar.DateAppearance.DateAfterFont.Height = -11
        Calendar.DateAppearance.DateAfterFont.Name = 'Tahoma'
        Calendar.DateAppearance.DateAfterFont.Style = []
        Calendar.StatusAppearance.Fill.Color = clRed
        Calendar.StatusAppearance.Fill.ColorMirror = clNone
        Calendar.StatusAppearance.Fill.ColorMirrorTo = clNone
        Calendar.StatusAppearance.Fill.GradientType = gtSolid
        Calendar.StatusAppearance.Fill.BorderColor = clGray
        Calendar.StatusAppearance.Fill.Rounding = 0
        Calendar.StatusAppearance.Fill.ShadowOffset = 0
        Calendar.StatusAppearance.Font.Charset = DEFAULT_CHARSET
        Calendar.StatusAppearance.Font.Color = clWhite
        Calendar.StatusAppearance.Font.Height = -11
        Calendar.StatusAppearance.Font.Name = 'Tahoma'
        Calendar.StatusAppearance.Font.Style = []
        Calendar.Footer.Fill.Color = 16773091
        Calendar.Footer.Fill.ColorTo = 16765615
        Calendar.Footer.Fill.ColorMirror = clNone
        Calendar.Footer.Fill.ColorMirrorTo = clNone
        Calendar.Footer.Fill.BorderColor = 16765615
        Calendar.Footer.Fill.Rounding = 0
        Calendar.Footer.Fill.ShadowOffset = 0
        Calendar.Footer.Font.Charset = DEFAULT_CHARSET
        Calendar.Footer.Font.Color = 7485192
        Calendar.Footer.Font.Height = -11
        Calendar.Footer.Font.Name = 'Tahoma'
        Calendar.Footer.Font.Style = []
        Calendar.Header.Fill.Color = 16773091
        Calendar.Header.Fill.ColorTo = 16765615
        Calendar.Header.Fill.ColorMirror = clNone
        Calendar.Header.Fill.ColorMirrorTo = clNone
        Calendar.Header.Fill.BorderColor = 16765615
        Calendar.Header.Fill.Rounding = 0
        Calendar.Header.Fill.ShadowOffset = 0
        Calendar.Header.ArrowColor = 7485192
        Calendar.Header.Font.Charset = DEFAULT_CHARSET
        Calendar.Header.Font.Color = 7485192
        Calendar.Header.Font.Height = -11
        Calendar.Header.Font.Name = 'Tahoma'
        Calendar.Header.Font.Style = []
        Calendar.Width = 257
        Calendar.Height = 249
        Calendar.ShowHint = False
        Date = 40213.000000000000000000
      end
      object procedure2_schedule: TAdvSmoothDatePicker
        Left = 712
        Top = 373
        Width = 108
        Height = 28
        Flat = False
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = []
        Lookup.Separator = ';'
        Color = clWindow
        Enabled = True
        TabOrder = 27
        Text = '2/4/2010'
        Visible = True
        Version = '1.5.4.1'
        ButtonStyle = bsDropDown
        ButtonWidth = 16
        Etched = False
        Glyph.Data = {
          DA020000424DDA0200000000000036000000280000000D0000000D0000000100
          200000000000A402000000000000000000000000000000000000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F00000000000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000000000000000000000000000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F0000000000000000000000000000000000000000000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F0000000000000000000000000000000
          0000000000000000000000000000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0
          F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000F0F0F000}
        HideCalendarAfterSelection = True
        Calendar.Fill.Color = 15984090
        Calendar.Fill.ColorTo = 15785680
        Calendar.Fill.ColorMirror = clNone
        Calendar.Fill.ColorMirrorTo = clNone
        Calendar.Fill.Rounding = 0
        Calendar.Fill.ShadowOffset = 0
        Calendar.Animation = True
        Calendar.ShowCurrentDate = True
        Calendar.DateAppearance.DateFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.DateFont.Color = clWindowText
        Calendar.DateAppearance.DateFont.Height = -11
        Calendar.DateAppearance.DateFont.Name = 'Tahoma'
        Calendar.DateAppearance.DateFont.Style = []
        Calendar.DateAppearance.DateFill.Color = 16773091
        Calendar.DateAppearance.DateFill.ColorTo = 16768452
        Calendar.DateAppearance.DateFill.ColorMirror = 16765357
        Calendar.DateAppearance.DateFill.ColorMirrorTo = 16767936
        Calendar.DateAppearance.DateFill.GradientMirrorType = gtVertical
        Calendar.DateAppearance.DateFill.Rounding = 0
        Calendar.DateAppearance.DateFill.ShadowOffset = 0
        Calendar.DateAppearance.DayOfWeekFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.DayOfWeekFont.Color = clWindowText
        Calendar.DateAppearance.DayOfWeekFont.Height = -11
        Calendar.DateAppearance.DayOfWeekFont.Name = 'Tahoma'
        Calendar.DateAppearance.DayOfWeekFont.Style = []
        Calendar.DateAppearance.DayOfWeekFill.Color = 15984090
        Calendar.DateAppearance.DayOfWeekFill.ColorTo = 15785680
        Calendar.DateAppearance.DayOfWeekFill.ColorMirror = clNone
        Calendar.DateAppearance.DayOfWeekFill.ColorMirrorTo = clNone
        Calendar.DateAppearance.DayOfWeekFill.Rounding = 0
        Calendar.DateAppearance.DayOfWeekFill.ShadowOffset = 0
        Calendar.DateAppearance.SelectedDateFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.SelectedDateFont.Color = clWindowText
        Calendar.DateAppearance.SelectedDateFont.Height = -11
        Calendar.DateAppearance.SelectedDateFont.Name = 'Tahoma'
        Calendar.DateAppearance.SelectedDateFont.Style = []
        Calendar.DateAppearance.SelectedDateFill.Color = 11196927
        Calendar.DateAppearance.SelectedDateFill.ColorTo = 7257087
        Calendar.DateAppearance.SelectedDateFill.ColorMirror = 4370174
        Calendar.DateAppearance.SelectedDateFill.ColorMirrorTo = 8053246
        Calendar.DateAppearance.SelectedDateFill.GradientMirrorType = gtVertical
        Calendar.DateAppearance.SelectedDateFill.BorderColor = 4370174
        Calendar.DateAppearance.SelectedDateFill.Rounding = 0
        Calendar.DateAppearance.SelectedDateFill.ShadowOffset = 0
        Calendar.DateAppearance.CurrentDateFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.CurrentDateFont.Color = clWindowText
        Calendar.DateAppearance.CurrentDateFont.Height = -11
        Calendar.DateAppearance.CurrentDateFont.Name = 'Tahoma'
        Calendar.DateAppearance.CurrentDateFont.Style = []
        Calendar.DateAppearance.CurrentDateFill.Color = 7778289
        Calendar.DateAppearance.CurrentDateFill.ColorTo = 4296947
        Calendar.DateAppearance.CurrentDateFill.ColorMirror = 946929
        Calendar.DateAppearance.CurrentDateFill.ColorMirrorTo = 5021693
        Calendar.DateAppearance.CurrentDateFill.GradientMirrorType = gtVertical
        Calendar.DateAppearance.CurrentDateFill.BorderColor = 4548219
        Calendar.DateAppearance.CurrentDateFill.Rounding = 0
        Calendar.DateAppearance.CurrentDateFill.ShadowOffset = 0
        Calendar.DateAppearance.WeekendFill.Color = 16773091
        Calendar.DateAppearance.WeekendFill.ColorTo = 16768452
        Calendar.DateAppearance.WeekendFill.ColorMirror = 16765357
        Calendar.DateAppearance.WeekendFill.ColorMirrorTo = 16767936
        Calendar.DateAppearance.WeekendFill.GradientMirrorType = gtVertical
        Calendar.DateAppearance.WeekendFill.Rounding = 0
        Calendar.DateAppearance.WeekendFill.ShadowOffset = 0
        Calendar.DateAppearance.WeekendFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.WeekendFont.Color = clWindowText
        Calendar.DateAppearance.WeekendFont.Height = -11
        Calendar.DateAppearance.WeekendFont.Name = 'Tahoma'
        Calendar.DateAppearance.WeekendFont.Style = []
        Calendar.DateAppearance.HoverDateFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.HoverDateFont.Color = clWindowText
        Calendar.DateAppearance.HoverDateFont.Height = -11
        Calendar.DateAppearance.HoverDateFont.Name = 'Tahoma'
        Calendar.DateAppearance.HoverDateFont.Style = []
        Calendar.DateAppearance.HoverDateFill.Color = 15465983
        Calendar.DateAppearance.HoverDateFill.ColorTo = 11332863
        Calendar.DateAppearance.HoverDateFill.ColorMirror = 5888767
        Calendar.DateAppearance.HoverDateFill.ColorMirrorTo = 10807807
        Calendar.DateAppearance.HoverDateFill.GradientMirrorType = gtVertical
        Calendar.DateAppearance.HoverDateFill.BorderColor = 10079963
        Calendar.DateAppearance.HoverDateFill.Rounding = 0
        Calendar.DateAppearance.HoverDateFill.ShadowOffset = 0
        Calendar.DateAppearance.MonthDateFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.MonthDateFont.Color = clWindowText
        Calendar.DateAppearance.MonthDateFont.Height = -11
        Calendar.DateAppearance.MonthDateFont.Name = 'Tahoma'
        Calendar.DateAppearance.MonthDateFont.Style = []
        Calendar.DateAppearance.YearDateFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.YearDateFont.Color = clWindowText
        Calendar.DateAppearance.YearDateFont.Height = -11
        Calendar.DateAppearance.YearDateFont.Name = 'Tahoma'
        Calendar.DateAppearance.YearDateFont.Style = []
        Calendar.DateAppearance.WeekNumbers.Font.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.WeekNumbers.Font.Color = clWindowText
        Calendar.DateAppearance.WeekNumbers.Font.Height = -11
        Calendar.DateAppearance.WeekNumbers.Font.Name = 'Tahoma'
        Calendar.DateAppearance.WeekNumbers.Font.Style = []
        Calendar.DateAppearance.WeekNumbers.Fill.Color = 15984090
        Calendar.DateAppearance.WeekNumbers.Fill.ColorTo = 15785680
        Calendar.DateAppearance.WeekNumbers.Fill.ColorMirror = clNone
        Calendar.DateAppearance.WeekNumbers.Fill.ColorMirrorTo = clNone
        Calendar.DateAppearance.WeekNumbers.Fill.Rounding = 0
        Calendar.DateAppearance.WeekNumbers.Fill.ShadowOffset = 0
        Calendar.DateAppearance.DisabledDateFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.DisabledDateFont.Color = clGray
        Calendar.DateAppearance.DisabledDateFont.Height = -11
        Calendar.DateAppearance.DisabledDateFont.Name = 'Tahoma'
        Calendar.DateAppearance.DisabledDateFont.Style = []
        Calendar.DateAppearance.DisabledDateFill.Color = 15921906
        Calendar.DateAppearance.DisabledDateFill.ColorTo = 11974326
        Calendar.DateAppearance.DisabledDateFill.ColorMirror = 11974326
        Calendar.DateAppearance.DisabledDateFill.ColorMirrorTo = 15921906
        Calendar.DateAppearance.DisabledDateFill.GradientMirrorType = gtVertical
        Calendar.DateAppearance.DisabledDateFill.Rounding = 0
        Calendar.DateAppearance.DisabledDateFill.ShadowOffset = 0
        Calendar.DateAppearance.DateBeforeFill.Color = clNone
        Calendar.DateAppearance.DateBeforeFill.ColorMirror = clNone
        Calendar.DateAppearance.DateBeforeFill.ColorMirrorTo = clNone
        Calendar.DateAppearance.DateBeforeFill.Rounding = 0
        Calendar.DateAppearance.DateBeforeFill.ShadowOffset = 0
        Calendar.DateAppearance.DateAfterFill.Color = clNone
        Calendar.DateAppearance.DateAfterFill.ColorMirror = clNone
        Calendar.DateAppearance.DateAfterFill.ColorMirrorTo = clNone
        Calendar.DateAppearance.DateAfterFill.Rounding = 0
        Calendar.DateAppearance.DateAfterFill.ShadowOffset = 0
        Calendar.DateAppearance.DateBeforeFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.DateBeforeFont.Color = clSilver
        Calendar.DateAppearance.DateBeforeFont.Height = -11
        Calendar.DateAppearance.DateBeforeFont.Name = 'Tahoma'
        Calendar.DateAppearance.DateBeforeFont.Style = []
        Calendar.DateAppearance.DateAfterFont.Charset = DEFAULT_CHARSET
        Calendar.DateAppearance.DateAfterFont.Color = clSilver
        Calendar.DateAppearance.DateAfterFont.Height = -11
        Calendar.DateAppearance.DateAfterFont.Name = 'Tahoma'
        Calendar.DateAppearance.DateAfterFont.Style = []
        Calendar.StatusAppearance.Fill.Color = clRed
        Calendar.StatusAppearance.Fill.ColorMirror = clNone
        Calendar.StatusAppearance.Fill.ColorMirrorTo = clNone
        Calendar.StatusAppearance.Fill.GradientType = gtSolid
        Calendar.StatusAppearance.Fill.BorderColor = clGray
        Calendar.StatusAppearance.Fill.Rounding = 0
        Calendar.StatusAppearance.Fill.ShadowOffset = 0
        Calendar.StatusAppearance.Font.Charset = DEFAULT_CHARSET
        Calendar.StatusAppearance.Font.Color = clWhite
        Calendar.StatusAppearance.Font.Height = -11
        Calendar.StatusAppearance.Font.Name = 'Tahoma'
        Calendar.StatusAppearance.Font.Style = []
        Calendar.Footer.Fill.Color = 16773091
        Calendar.Footer.Fill.ColorTo = 16765615
        Calendar.Footer.Fill.ColorMirror = clNone
        Calendar.Footer.Fill.ColorMirrorTo = clNone
        Calendar.Footer.Fill.BorderColor = 16765615
        Calendar.Footer.Fill.Rounding = 0
        Calendar.Footer.Fill.ShadowOffset = 0
        Calendar.Footer.Font.Charset = DEFAULT_CHARSET
        Calendar.Footer.Font.Color = 7485192
        Calendar.Footer.Font.Height = -11
        Calendar.Footer.Font.Name = 'Tahoma'
        Calendar.Footer.Font.Style = []
        Calendar.Header.Fill.Color = 16773091
        Calendar.Header.Fill.ColorTo = 16765615
        Calendar.Header.Fill.ColorMirror = clNone
        Calendar.Header.Fill.ColorMirrorTo = clNone
        Calendar.Header.Fill.BorderColor = 16765615
        Calendar.Header.Fill.Rounding = 0
        Calendar.Header.Fill.ShadowOffset = 0
        Calendar.Header.ArrowColor = 7485192
        Calendar.Header.Font.Charset = DEFAULT_CHARSET
        Calendar.Header.Font.Color = 7485192
        Calendar.Header.Font.Height = -11
        Calendar.Header.Font.Name = 'Tahoma'
        Calendar.Header.Font.Style = []
        Calendar.Width = 257
        Calendar.Height = 249
        Calendar.ShowHint = False
        Date = 40213.000000000000000000
      end
      object StaticText2: TStaticText
        Left = 72
        Top = 40
        Width = 111
        Height = 17
        Caption = 'Letter && numbers only'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 28
      end
    end
  end
  object StaticText1: TStaticText
    Left = 208
    Top = 575
    Width = 115
    Height = 28
    Caption = 'Selected bird'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
  end
  object name_view: TEdit
    Left = 328
    Top = 575
    Width = 145
    Height = 28
    Color = clYellow
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    ReadOnly = True
    TabOrder = 2
  end
  object OKBtn: TButton
    Left = 494
    Top = 578
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 3
    OnClick = OKBtnClick
  end
  object CancelBtn: TButton
    Left = 575
    Top = 578
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 4
  end
  object BitBtn17: TBitBtn
    Left = 656
    Top = 578
    Width = 77
    Height = 25
    Caption = 'Help'
    DoubleBuffered = True
    Glyph.Data = {
      76010000424D7601000000000000760000002800000020000000100000000100
      04000000000000010000120B0000120B00001000000000000000000000000000
      800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
      3333333333FFFFF3333333333F797F3333333333F737373FF333333BFB999BFB
      33333337737773773F3333BFBF797FBFB33333733337333373F33BFBFBFBFBFB
      FB3337F33333F33337F33FBFBFB9BFBFBF3337333337F333373FFBFBFBF97BFB
      FBF37F333337FF33337FBFBFBFB99FBFBFB37F3333377FF3337FFBFBFBFB99FB
      FBF37F33333377FF337FBFBF77BF799FBFB37F333FF3377F337FFBFB99FB799B
      FBF373F377F3377F33733FBF997F799FBF3337F377FFF77337F33BFBF99999FB
      FB33373F37777733373333BFBF999FBFB3333373FF77733F7333333BFBFBFBFB
      3333333773FFFF77333333333FBFBF3333333333377777333333}
    NumGlyphs = 2
    ParentDoubleBuffered = False
    TabOrder = 5
    OnClick = BitBtn17Click
  end
  object DataSource1: TDataSource
    DataSet = DataForm.birds
    Left = 832
    Top = 224
  end
end
