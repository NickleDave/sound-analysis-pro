object Frame1: TFrame1
  Left = 0
  Top = 0
  Width = 391
  Height = 235
  TabOrder = 0
  object segment_panels: TPanel
    Left = 3
    Top = 3
    Width = 382
    Height = 230
    BevelInner = bvLowered
    TabOrder = 0
    object hp_small_num: TNumLab
      Left = 325
      Top = 56
      Width = 35
      Height = 17
      Alignment = taCenter
      ColorLabBakG = clSilver
      ColorLabText = clMaroon
      ColorOutBakG = clSilver
      ColorOutText = clBlack
      ColorScheme = csBWG
      Comma = False
      DisplayType = dtFixP
      DTFormat = 'mmm-dd, yyyy'
      Empty = False
      ForcePlusSign = False
      FrameStyle = fsLowered
      LabelWidth = 33
      LeftSpace = 0
      LeftTextAlignment = taRightJustify
      OverflowIndicator = '*********'
      Precision = 0
      RightTextAlignment = taLeftJustify
      StateText = 'undefined'
      Transparent = False
      Value = 50.000000000000000000
    end
    object hp_big_num: TNumLab
      Left = 325
      Top = 75
      Width = 35
      Height = 17
      Alignment = taCenter
      ColorLabBakG = clSilver
      ColorLabText = clMaroon
      ColorOutBakG = clSilver
      ColorOutText = clBlack
      ColorScheme = csBWG
      Comma = False
      DisplayType = dtFixP
      DTFormat = 'mmm-dd, yyyy'
      Empty = False
      ForcePlusSign = False
      FrameStyle = fsLowered
      LabelWidth = 33
      LeftSpace = 0
      LeftTextAlignment = taRightJustify
      OverflowIndicator = '*********'
      Precision = 0
      RightTextAlignment = taLeftJustify
      StateText = 'undefined'
      Transparent = False
      Value = 500.000000000000000000
    end
    object NumLab3: TNumLab
      Left = 267
      Top = 29
      Width = 60
      Height = 17
      Alignment = taCenter
      ColorLabBakG = clYellow
      ColorLabText = clBlack
      ColorOutBakG = clSilver
      ColorOutText = clBlack
      ColorScheme = csBWG
      Comma = False
      DisplayType = dtFixP
      DTFormat = 'mmm-dd, yyyy'
      Empty = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ForcePlusSign = False
      FrameStyle = fsLowered
      LabelWidth = 60
      LeftSpace = 0
      LeftTextAlignment = taRightJustify
      OverflowIndicator = '*********'
      ParentFont = False
      Precision = 2
      RightTextAlignment = taLeftJustify
      StateText = 'undefined'
      Transparent = False
      Value = 64.000000000000000000
    end
    object min_stop_durN: TNumLab
      Left = 210
      Top = 180
      Width = 40
      Height = 17
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
      ForcePlusSign = False
      FrameStyle = fsLowered
      LabelWidth = 35
      LeftSpace = 0
      LeftTextAlignment = taRightJustify
      OverflowIndicator = '*********'
      Precision = 0
      RightTextAlignment = taLeftJustify
      StateText = 'undefined'
      Transparent = False
      Value = 7.000000000000000000
    end
    object bout_Stop_N: TNumLab
      Left = 210
      Top = 204
      Width = 40
      Height = 17
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
      ForcePlusSign = False
      FrameStyle = fsLowered
      LabelWidth = 35
      LeftSpace = 0
      LeftTextAlignment = taRightJustify
      OverflowIndicator = '*********'
      Precision = 0
      RightTextAlignment = taLeftJustify
      StateText = 'undefined'
      Transparent = False
      Value = 100.000000000000000000
    end
    object segmentation_flip: TRadioGroup
      Left = 206
      Top = 3
      Width = 50
      Height = 50
      ItemIndex = 0
      Items.Strings = (
        '>'
        '<')
      TabOrder = 0
      OnClick = segmentation_flipClick
    end
    object segment_by: TComboBox
      Left = 5
      Top = 29
      Width = 153
      Height = 21
      AutoComplete = False
      ItemIndex = 0
      TabOrder = 1
      Text = 'Amplitude'
      OnChange = segment_byChange
      Items.Strings = (
        'Amplitude'
        'Pitch'
        'Mean frequency'
        'Goodness of pitch'
        'FM'
        'AM'
        'Wiener entropy'
        'Continuity (t)'
        'Continuity (f)')
    end
    object StaticText32: TStaticText
      Left = 5
      Top = 3
      Width = 200
      Height = 20
      Caption = 'Segment by Primary Feature'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 2
    end
    object smoothGraphCB: TCheckBox
      Left = 5
      Top = 56
      Width = 113
      Height = 17
      Caption = 'Smooth feature'
      TabOrder = 3
      OnClick = smoothGraphCBClick
    end
    object hp_small: TTrackBar
      Left = 234
      Top = 52
      Width = 80
      Height = 25
      Max = 100
      Frequency = 10
      Position = 50
      TabOrder = 4
      ThumbLength = 15
      OnChange = hp_smallChange
    end
    object StaticText49: TStaticText
      Left = 136
      Top = 56
      Width = 76
      Height = 17
      Caption = 'Smooth feature'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 5
    end
    object adaptThreshCB: TCheckBox
      Left = 5
      Top = 75
      Width = 113
      Height = 17
      Caption = 'Adapt Threshold'
      TabOrder = 6
      OnClick = adaptThresholdCBClick
    end
    object StaticText50: TStaticText
      Left = 136
      Top = 75
      Width = 92
      Height = 17
      Caption = 'Adaptive threshold'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 7
    end
    object hp_big: TTrackBar
      Left = 234
      Top = 75
      Width = 80
      Height = 25
      Max = 900
      Min = 200
      Frequency = 50
      Position = 500
      TabOrder = 8
      ThumbLength = 15
      OnChange = hp_bigChange
    end
    object secondary_segmentation_panel: TPanel
      Left = 3
      Top = 98
      Width = 371
      Height = 76
      BevelInner = bvRaised
      BevelOuter = bvLowered
      TabOrder = 9
      object secondaryThresholdTB: TNumLab
        Left = 218
        Top = 49
        Width = 60
        Height = 17
        Alignment = taCenter
        ColorLabBakG = clAqua
        ColorLabText = clBlack
        ColorOutBakG = clAqua
        ColorOutText = clBlack
        ColorScheme = csBWG
        Comma = False
        DisplayType = dtFixP
        DTFormat = 'mmm-dd, yyyy'
        Empty = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ForcePlusSign = False
        FrameStyle = fsLowered
        LabelWidth = 60
        LeftSpace = 0
        LeftTextAlignment = taRightJustify
        OverflowIndicator = '*********'
        ParentFont = False
        Precision = 2
        RightTextAlignment = taLeftJustify
        StateText = 'undefined'
        Transparent = False
        Value = 64.000000000000000000
      end
      object secondaryFeatureDD: TComboBox
        Left = 3
        Top = 37
        Width = 153
        Height = 21
        AutoComplete = False
        ItemIndex = 0
        TabOrder = 0
        Text = 'Amplitude'
        OnChange = secondaryFeatureDDChange
        Items.Strings = (
          'Amplitude'
          'Pitch'
          'Mean frequency'
          'Goodness of pitch'
          'FM'
          'AM'
          'Wiener entropy'
          'Continuity (t)'
          'Continuity (f)')
      end
      object secondaryComparisonRB: TRadioGroup
        Left = 162
        Top = 27
        Width = 50
        Height = 43
        ItemIndex = 0
        Items.Strings = (
          '>'
          '<')
        TabOrder = 1
        OnClick = secondaryComparisonRBClick
      end
      object secondaryLogicRB: TRadioGroup
        Left = 249
        Top = 12
        Width = 108
        Height = 31
        Columns = 2
        ItemIndex = 0
        Items.Strings = (
          'AND'
          'OR')
        TabOrder = 2
        OnClick = secondaryLogicRBClick
      end
      object secondaryFeatureCB: TCheckBox
        Left = 16
        Top = 6
        Width = 209
        Height = 25
        Caption = 'Segment by Secondary Feature'
        TabOrder = 3
        OnClick = secondaryFeatureCBClick
      end
    end
    object StaticText6: TStaticText
      Left = 254
      Top = 180
      Width = 17
      Height = 17
      Caption = 'ms'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 10
    end
    object StaticText29: TStaticText
      Left = 254
      Top = 204
      Width = 17
      Height = 17
      Caption = 'ms'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 11
    end
    object min_stop_durTb: TTrackBar
      Left = 129
      Top = 180
      Width = 80
      Height = 25
      Max = 100
      Min = 1
      Frequency = 10
      Position = 5
      TabOrder = 12
      ThumbLength = 14
      OnChange = min_stop_durTbChange
    end
    object bout_stop: TTrackBar
      Left = 129
      Top = 204
      Width = 80
      Height = 25
      Max = 200
      Min = 10
      Frequency = 10
      Position = 71
      TabOrder = 13
      ThumbLength = 14
      OnChange = bout_stopChange
    end
    object StaticText30: TStaticText
      Left = 12
      Top = 180
      Width = 110
      Height = 18
      Caption = 'Minimum stop duration'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = #39#39#39#39#39#39#39
      Font.Style = []
      ParentFont = False
      TabOrder = 14
    end
    object StaticText54: TStaticText
      Left = 10
      Top = 204
      Width = 113
      Height = 17
      Caption = 'Bout ends when stop>'
      TabOrder = 15
    end
    object save: TBitBtn
      Left = 299
      Top = 204
      Width = 75
      Height = 25
      Caption = 'Save'
      DoubleBuffered = True
      ParentDoubleBuffered = False
      TabOrder = 16
      OnClick = saveClick
    end
  end
end
