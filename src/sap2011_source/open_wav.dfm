object openwav: Topenwav
  Left = 0
  Top = 0
  Caption = 'open_wav'
  ClientHeight = 406
  ClientWidth = 413
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Visible = True
  PixelsPerInch = 96
  TextHeight = 13
  object Image1: TImage
    Left = 8
    Top = 232
    Width = 400
    Height = 100
  end
  object DriveComboBox1: TDriveComboBox
    Left = 8
    Top = 8
    Width = 185
    Height = 19
    DirList = DirectoryListBox1
    TabOrder = 0
  end
  object DirectoryListBox1: TDirectoryListBox
    Left = 8
    Top = 33
    Width = 185
    Height = 176
    FileList = sound_list
    TabOrder = 1
  end
  object sound_list: TFileListBox
    Left = 199
    Top = 8
    Width = 207
    Height = 201
    ItemHeight = 13
    Mask = '*.wav'
    TabOrder = 2
    OnClick = sound_listClick
  end
  object OKBtn: TButton
    Left = 89
    Top = 359
    Width = 123
    Height = 40
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'OK'
    Default = True
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ModalResult = 1
    ParentFont = False
    TabOrder = 3
    OnClick = OKBtnClick
  end
  object CancelBtn: TButton
    Left = 223
    Top = 359
    Width = 124
    Height = 40
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Cancel = True
    Caption = 'Cancel'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ModalResult = 2
    ParentFont = False
    TabOrder = 4
    OnClick = CancelBtnClick
  end
  object invalid: TStaticText
    Left = 136
    Top = 264
    Width = 139
    Height = 27
    Caption = 'Invalid wave file'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    Visible = False
  end
  object gain: TTrackBar
    Left = 8
    Top = 338
    Width = 88
    Height = 20
    Max = 30
    Min = 1
    Frequency = 100
    Position = 5
    TabOrder = 6
    ThumbLength = 18
    TickStyle = tsNone
    OnChange = sound_listClick
  end
end
