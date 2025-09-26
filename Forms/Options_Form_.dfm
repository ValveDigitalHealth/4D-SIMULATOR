object Options_Form: TOptions_Form
  Left = 0
  Top = 0
  Caption = 'Options_Form'
  ClientHeight = 462
  ClientWidth = 1019
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 144
  TextHeight = 19
  object Label1: TLabel
    Left = 16
    Top = 32
    Width = 221
    Height = 19
    Caption = 'Range for unipolar electrogram'
  end
  object Label2: TLabel
    Left = 16
    Top = 62
    Width = 196
    Height = 19
    Caption = 'Bipolar electrogram spacing'
  end
  object Range_for_Uni_El_Edit: TEdit
    Left = 243
    Top = 29
    Width = 46
    Height = 27
    TabOrder = 0
    Text = '10'
  end
  object Close_Button: TButton
    Left = 784
    Top = 328
    Width = 185
    Height = 97
    Caption = 'CLOSE'
    TabOrder = 1
    OnClick = Close_ButtonClick
  end
  object Bip_Spacing_Edit: TEdit
    Left = 243
    Top = 57
    Width = 46
    Height = 27
    TabOrder = 2
    Text = '5'
  end
end
