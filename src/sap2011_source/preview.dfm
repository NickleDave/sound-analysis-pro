object previewSound: TpreviewSound
  Left = 0
  Top = 0
  Caption = 'Preview Sound'
  ClientHeight = 140
  ClientWidth = 912
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Image1: TImage
    Left = 8
    Top = 8
    Width = 833
    Height = 124
    OnMouseDown = Image1MouseDown
  end
  object Button1: TButton
    Left = 847
    Top = 107
    Width = 25
    Height = 25
    Caption = 'Button1'
    TabOrder = 0
    Visible = False
    OnClick = Button1Click
  end
end
