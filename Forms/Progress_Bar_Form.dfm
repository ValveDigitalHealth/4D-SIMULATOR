object Progress_Form: TProgress_Form
  Left = 776
  Top = 384
  ClientHeight = 121
  ClientWidth = 642
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  PixelsPerInch = 144
  TextHeight = 20
  object ProgressBar1: TProgressBar
    Left = 16
    Top = 16
    Width = 489
    Height = 49
    Step = 1
    TabOrder = 0
  end
  object Stop_Button: TButton
    Left = 520
    Top = 16
    Width = 97
    Height = 49
    Caption = 'STOP'
    TabOrder = 1
    OnClick = Stop_ButtonClick
  end
end
