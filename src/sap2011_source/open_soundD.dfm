inherited open_sound: Topen_sound
  ClientHeight = 497
  ClientWidth = 710
  OnActivate = FormActivate
  ExplicitWidth = 716
  ExplicitHeight = 525
  PixelsPerInch = 96
  TextHeight = 13
  inherited Bevel1: TBevel
    Width = 455
    Height = 473
    ExplicitWidth = 455
    ExplicitHeight = 473
  end
  object Image1: TImage [1]
    Left = 8
    Top = 339
    Width = 417
    Height = 124
    Stretch = True
  end
  object Label1: TLabel [2]
    Left = 25
    Top = 280
    Width = 92
    Height = 16
    Caption = 'Duration (secs):'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object duration: TLabel [3]
    Left = 139
    Top = 280
    Width = 7
    Height = 16
    Caption = '0'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel [4]
    Left = 272
    Top = 313
    Width = 44
    Height = 13
    Caption = 'contrast:'
  end
  object Label3: TLabel [5]
    Left = 210
    Top = 281
    Width = 85
    Height = 16
    Caption = 'Sampling rate:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object sampling: TLabel [6]
    Left = 319
    Top = 281
    Width = 5
    Height = 16
    Caption = '-'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object frequency_range_val: TNumLab [7]
    Left = 623
    Top = 254
    Width = 48
    Height = 17
    Alignment = taCenter
    ColorLabBakG = clSilver
    ColorLabText = clNavy
    ColorOutBakG = clSilver
    ColorOutText = clBlack
    ColorScheme = csBWG
    Comma = False
    DisplayType = dtFixP
    DTFormat = 'mmm-dd, yyyy'
    Empty = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -12
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ForcePlusSign = False
    FrameStyle = fsLowered
    LabelWidth = 38
    LeftSpace = 0
    LeftTextAlignment = taRightJustify
    OverflowIndicator = '*********'
    ParentFont = False
    Precision = 0
    RightTextAlignment = taLeftJustify
    StateText = 'undefined'
    Transparent = False
    Value = 10000.000000000000000000
  end
  object windowTB: TNumLab [8]
    Left = 623
    Top = 292
    Width = 40
    Height = 17
    Alignment = taCenter
    ColorLabBakG = clSilver
    ColorLabText = clNavy
    ColorOutBakG = clSilver
    ColorOutText = clBlack
    ColorScheme = csBWG
    Comma = False
    DisplayType = dtFixP
    DTFormat = 'mmm-dd, yyyy'
    Empty = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -12
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ForcePlusSign = False
    FrameStyle = fsLowered
    LabelWidth = 38
    LeftSpace = 0
    LeftTextAlignment = taRightJustify
    OverflowIndicator = '*********'
    ParentFont = False
    Precision = 2
    RightTextAlignment = taLeftJustify
    StateText = 'undefined'
    Transparent = False
    Value = 9.270000000000000000
  end
  object advanceTb: TNumLab [9]
    Left = 623
    Top = 339
    Width = 40
    Height = 17
    Alignment = taCenter
    ColorLabBakG = clSilver
    ColorLabText = clNavy
    ColorOutBakG = clSilver
    ColorOutText = clBlack
    ColorScheme = csBWG
    Comma = False
    DisplayType = dtFixP
    DTFormat = 'mmm-dd, yyyy'
    Empty = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -12
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ForcePlusSign = False
    FrameStyle = fsLowered
    LabelWidth = 38
    LeftSpace = 0
    LeftTextAlignment = taRightJustify
    OverflowIndicator = '*********'
    ParentFont = False
    Precision = 2
    RightTextAlignment = taLeftJustify
    StateText = 'undefined'
    Transparent = False
    Value = 1.000000000000000000
  end
  object paginate_val: TNumLab [10]
    Left = 608
    Top = 395
    Width = 40
    Height = 17
    Alignment = taCenter
    ColorLabBakG = clSilver
    ColorLabText = clNavy
    ColorOutBakG = clSilver
    ColorOutText = clBlack
    ColorScheme = csBWG
    Comma = False
    DisplayType = dtFixP
    DTFormat = 'mmm-dd, yyyy'
    Empty = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -12
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ForcePlusSign = False
    FrameStyle = fsLowered
    LabelWidth = 38
    LeftSpace = 0
    LeftTextAlignment = taRightJustify
    OverflowIndicator = '*********'
    ParentFont = False
    Precision = 0
    RightTextAlignment = taLeftJustify
    StateText = 'undefined'
    Transparent = False
    Value = 5.000000000000000000
  end
  inherited OKBtn: TButton
    Left = 471
    Top = 469
    Enabled = False
    OnClick = OKBtnClick
    ExplicitLeft = 471
    ExplicitTop = 469
  end
  inherited CancelBtn: TButton
    Left = 551
    Top = 469
    OnClick = CancelBtnClick
    ExplicitLeft = 551
    ExplicitTop = 469
  end
  object HelpBtn: TButton
    Left = 631
    Top = 469
    Width = 75
    Height = 25
    Caption = '&Help'
    TabOrder = 2
    OnClick = HelpBtnClick
  end
  object DriveComboBox1: TDriveComboBox
    Left = 16
    Top = 39
    Width = 153
    Height = 19
    DirList = DirectoryListBox1
    TabOrder = 3
  end
  object DirectoryListBox1: TDirectoryListBox
    Left = 16
    Top = 65
    Width = 153
    Height = 183
    FileList = sound_list
    TabOrder = 4
  end
  object sound_list: TFileListBox
    Left = 175
    Top = 8
    Width = 282
    Height = 241
    ItemHeight = 13
    Mask = '*.wav'
    TabOrder = 5
    OnClick = sound_listClick
    OnDblClick = sound_listDblClick
  end
  object invalid: TStaticText
    Left = 25
    Top = 362
    Width = 382
    Height = 67
    AutoSize = False
    Caption = '               Cannot open wave file'
    Color = clYellow
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentColor = False
    ParentFont = False
    TabOrder = 6
    Transparent = False
    Visible = False
  end
  object gain: TTrackBar
    Left = 322
    Top = 311
    Width = 88
    Height = 20
    Max = 30
    Min = 1
    Frequency = 100
    Position = 5
    TabOrder = 7
    ThumbLength = 18
    TickStyle = tsNone
    OnChange = sound_listClick
  end
  object search_file: TBitBtn
    Left = 56
    Top = 12
    Width = 91
    Height = 25
    Caption = 'Search file'
    DoubleBuffered = True
    ParentDoubleBuffered = False
    TabOrder = 8
    OnClick = search_fileClick
  end
  object too_long: TStaticText
    Left = 46
    Top = 362
    Width = 217
    Height = 39
    AutoSize = False
    Caption = 
      'warning: this file might be too long and will take a while to an' +
      'alyze'
    Color = clYellow
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentColor = False
    ParentFont = False
    TabOrder = 9
    Transparent = False
    Visible = False
  end
  object warning: TCheckBox
    Left = 25
    Top = 254
    Width = 97
    Height = 17
    Caption = 'Show warning'
    Checked = True
    State = cbChecked
    TabOrder = 10
    OnClick = warningClick
  end
  object play: TMediaPlayer
    Left = 19
    Top = 312
    Width = 85
    Height = 20
    ColoredButtons = [btPlay, btPause, btStop, btPrev, btStep, btBack, btRecord, btEject]
    EnabledButtons = [btPlay, btPause, btStop]
    VisibleButtons = [btPlay, btPause, btStop]
    DeviceType = dtWaveAudio
    DoubleBuffered = False
    ParentDoubleBuffered = False
    TabOrder = 11
  end
  object playbacks: TCheckBox
    Left = 110
    Top = 312
    Width = 97
    Height = 17
    Caption = 'enable playbacks'
    TabOrder = 12
    OnClick = warningClick
  end
  object frequency_range: TTrackBar
    Left = 471
    Top = 254
    Width = 134
    Height = 25
    Max = 3
    Min = 1
    PageSize = 1
    Position = 2
    TabOrder = 13
    ThumbLength = 15
    OnChange = frequency_rangeChange
  end
  object StaticText144: TStaticText
    Left = 479
    Top = 228
    Width = 103
    Height = 21
    Caption = 'Frequency range'
    Font.Charset = ANSI_CHARSET
    Font.Color = clNavy
    Font.Height = -15
    Font.Name = 'Times New Roman'
    Font.Style = []
    ParentFont = False
    TabOrder = 14
  end
  object StaticText126: TStaticText
    Left = 677
    Top = 254
    Width = 17
    Height = 17
    Caption = 'Hz'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -12
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 15
  end
  object StaticText158: TStaticText
    Left = 475
    Top = 274
    Width = 108
    Height = 21
    Caption = 'FFT data window'
    Font.Charset = ANSI_CHARSET
    Font.Color = clNavy
    Font.Height = -15
    Font.Name = 'Times New Roman'
    Font.Style = []
    ParentFont = False
    TabOrder = 16
  end
  object data_window: TTrackBar
    Left = 475
    Top = 295
    Width = 130
    Height = 25
    Hint = 
      'longer window -> better frequency resolution, shorter window -> ' +
      'better time resolution'
    Max = 1023
    Min = 20
    ParentShowHint = False
    Frequency = 50
    Position = 409
    ShowHint = False
    TabOrder = 17
    ThumbLength = 15
    OnChange = data_windowChange
  end
  object StaticText157: TStaticText
    Left = 669
    Top = 292
    Width = 17
    Height = 17
    Caption = 'ms'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -12
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 18
  end
  object StaticText106: TStaticText
    Left = 476
    Top = 322
    Width = 107
    Height = 21
    Caption = 'Advance window'
    Font.Charset = ANSI_CHARSET
    Font.Color = clNavy
    Font.Height = -15
    Font.Name = 'Times New Roman'
    Font.Style = []
    ParentFont = False
    TabOrder = 19
  end
  object advance: TTrackBar
    Left = 474
    Top = 345
    Width = 131
    Height = 25
    Hint = 'more advance = less overlap = more compressed sonogram'
    Max = 500
    Min = 10
    ParentShowHint = False
    Frequency = 50
    Position = 44
    ShowHint = False
    TabOrder = 20
    ThumbLength = 15
    OnChange = advanceChange
  end
  object StaticText1: TStaticText
    Left = 669
    Top = 339
    Width = 17
    Height = 17
    Caption = 'ms'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -12
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 21
  end
  object pagination: TRadioGroup
    Left = 469
    Top = 425
    Width = 230
    Height = 38
    Caption = 'Pagination'
    Columns = 2
    Font.Charset = ANSI_CHARSET
    Font.Color = clNavy
    Font.Height = -12
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ItemIndex = 1
    Items.Strings = (
      'full file'
      'page by page')
    ParentFont = False
    TabOrder = 22
    OnClick = paginationClick
  end
  object file_comments: TMemo
    Left = 471
    Top = 26
    Width = 201
    Height = 159
    TabOrder = 23
  end
  object StaticText226: TStaticText
    Left = 471
    Top = 8
    Width = 134
    Height = 20
    Caption = 'Type comments here:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 24
  end
  object save_comments: TBitBtn
    Left = 471
    Top = 187
    Width = 201
    Height = 35
    Caption = 'Save comments'
    DoubleBuffered = True
    Font.Charset = ANSI_CHARSET
    Font.Color = clNavy
    Font.Height = -15
    Font.Name = 'Comic Sans MS'
    Font.Style = []
    Glyph.Data = {
      76010000424D7601000000000000760000002800000020000000100000000100
      04000000000000010000120B0000120B00001000000000000000000000000000
      800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
      333333FFFFFFFFFFFFF33000077777770033377777777777773F000007888888
      00037F3337F3FF37F37F00000780088800037F3337F77F37F37F000007800888
      00037F3337F77FF7F37F00000788888800037F3337777777337F000000000000
      00037F3FFFFFFFFFFF7F00000000000000037F77777777777F7F000FFFFFFFFF
      00037F7F333333337F7F000FFFFFFFFF00037F7F333333337F7F000FFFFFFFFF
      00037F7F333333337F7F000FFFFFFFFF00037F7F333333337F7F000FFFFFFFFF
      00037F7F333333337F7F000FFFFFFFFF07037F7F33333333777F000FFFFFFFFF
      0003737FFFFFFFFF7F7330099999999900333777777777777733}
    NumGlyphs = 2
    ParentDoubleBuffered = False
    ParentFont = False
    TabOrder = 25
    OnClick = save_commentsClick
  end
  object file_edit: TEdit
    Left = 19
    Top = 195
    Width = 395
    Height = 21
    Color = clBtnFace
    ReadOnly = True
    TabOrder = 26
    Text = '*.wav'
    Visible = False
    OnChange = file_editChange
  end
  object paginate_tb: TTrackBar
    Left = 471
    Top = 395
    Width = 131
    Height = 25
    Hint = 'more advance = less overlap = more compressed sonogram'
    Max = 60
    Min = 1
    ParentShowHint = False
    Frequency = 50
    Position = 5
    ShowHint = False
    TabOrder = 27
    ThumbLength = 15
    OnChange = paginate_tbChange
  end
  object StaticText2: TStaticText
    Left = 471
    Top = 376
    Width = 177
    Height = 21
    Caption = 'Auto paginate if sound > than'
    Font.Charset = ANSI_CHARSET
    Font.Color = clNavy
    Font.Height = -15
    Font.Name = 'Times New Roman'
    Font.Style = []
    ParentFont = False
    TabOrder = 28
  end
  object StaticText3: TStaticText
    Left = 654
    Top = 395
    Width = 24
    Height = 19
    Caption = 'sec'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -12
    Font.Name = 'Sec'
    Font.Style = []
    ParentFont = False
    TabOrder = 29
  end
  object open: TFileOpenDialog
    FavoriteLinks = <>
    FileTypes = <>
    Options = []
    Left = 296
    Top = 136
  end
end
