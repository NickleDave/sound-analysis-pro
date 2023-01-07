inherited OKHelpBottomDlg: TOKHelpBottomDlg
  PixelsPerInch = 96
  TextHeight = 13
  inherited OKBtn: TButton
    Left = 70
    ExplicitLeft = 70
  end
  inherited CancelBtn: TButton
    Left = 150
    ExplicitLeft = 150
  end
  object HelpBtn: TButton
    Left = 230
    Top = 180
    Width = 75
    Height = 25
    Caption = '&Help'
    TabOrder = 2
    OnClick = HelpBtnClick
  end
end
