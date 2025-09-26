object Main_Screen_Form: TMain_Screen_Form
  Left = 0
  Top = 0
  Align = alClient
  Caption = '4D active media simulator'
  ClientHeight = 1030
  ClientWidth = 1924
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnResize = FormResize
  OnShow = FormShow
  TextHeight = 13
  object Main_PageControl: TPageControl
    Left = 239
    Top = 29
    Width = 898
    Height = 826
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    ActivePage = TabSheet2
    TabOrder = 0
    object TabSheet1: TTabSheet
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'Main controls'
      object Time_Label: TLabel
        Left = 13
        Top = 125
        Width = 26
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Time:'
      end
      object Timer_Slider_Label: TLabel
        Left = 13
        Top = 300
        Width = 104
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Voltage history slider:'
      end
      object Label56: TLabel
        Left = 310
        Top = 244
        Width = 119
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Direction tolerance [deg]'
      end
      object Label36: TLabel
        Left = 234
        Top = 122
        Width = 53
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Pause [ms]'
      end
      object Label57: TLabel
        Left = 234
        Top = 97
        Width = 67
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Timestep [ms]'
      end
      object Label58: TLabel
        Left = 150
        Top = 244
        Width = 64
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Mesh spacing'
      end
      object T_Label: TLabel
        Left = 203
        Top = 394
        Width = 26
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Step:'
      end
      object Start_Sim_Button: TButton
        Left = 11
        Top = 93
        Width = 61
        Height = 28
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'START'
        TabOrder = 0
        OnClick = Start_Sim_ButtonClick
      end
      object Stop_Sim_Button: TButton
        Left = 76
        Top = 93
        Width = 61
        Height = 28
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'STOP'
        TabOrder = 1
        OnClick = Stop_Sim_ButtonClick
      end
      object Stim_Button: TButton
        Left = 11
        Top = 153
        Width = 61
        Height = 34
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Stimulate'
        TabOrder = 2
        OnClick = Stim_ButtonClick
      end
      object Clear_Button: TButton
        Left = 141
        Top = 93
        Width = 57
        Height = 28
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Clear'
        TabOrder = 3
        OnClick = Clear_ButtonClick
      end
      object Make_Spiral_In_Current_Slice_Button: TButton
        Left = 76
        Top = 153
        Width = 94
        Height = 34
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'SPIRAL'
        TabOrder = 4
        OnClick = Make_Spiral_In_Current_Slice_ButtonClick
      end
      object System_Dim_RadioGroup: TRadioGroup
        Left = 2
        Top = 2
        Width = 116
        Height = 87
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'System dimension'
        ItemIndex = 0
        Items.Strings = (
          '2D'
          '3D'
          '4D'
          '2 layers')
        TabOrder = 5
        OnClick = System_Dim_RadioGroupClick
      end
      object Boundary_Conditions_RadioGroup: TRadioGroup
        Left = 122
        Top = 2
        Width = 117
        Height = 51
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Boundary conditions'
        ItemIndex = 0
        Items.Strings = (
          'Zero-flux'
          'Periodic')
        TabOrder = 6
        OnClick = Boundary_Conditions_RadioGroupClick
      end
      object LMB_Action_RadioGroup: TRadioGroup
        Left = 257
        Top = 2
        Width = 127
        Height = 87
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Left mouse button'
        ItemIndex = 2
        Items.Strings = (
          'Stimulate'
          'Ablate'
          'Point to see signal'
          'Draw line')
        TabOrder = 7
      end
      object Layer_RadioGroup: TRadioGroup
        Left = 13
        Top = 197
        Width = 114
        Height = 88
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Layer'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = []
        ItemIndex = 1
        Items.Strings = (
          '1'
          '2')
        ParentFont = False
        TabOrder = 8
        OnClick = Layer_RadioGroupClick
      end
      object Initiate_AF_Button: TButton
        Left = 174
        Top = 153
        Width = 95
        Height = 34
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Initiate AF'
        TabOrder = 9
        OnClick = Initiate_AF_ButtonClick
      end
      object History_ScrollBar: TScrollBar
        Left = 13
        Top = 317
        Width = 371
        Height = 18
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Max = 1000
        PageSize = 0
        TabOrder = 10
        OnChange = History_ScrollBarChange
      end
      object Calculate_CV_Button: TButton
        Left = 149
        Top = 266
        Width = 139
        Height = 34
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Calculate CV arrows'
        TabOrder = 11
        OnClick = Calculate_CV_ButtonClick
      end
      object Laminar_Domains_Calculate_Button: TButton
        Left = 292
        Top = 266
        Width = 165
        Height = 34
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Recalculate laminar domains'
        TabOrder = 12
        OnClick = Laminar_Domains_Calculate_ButtonClick
      end
      object Direction_Tolerance_Edit: TEdit
        Left = 433
        Top = 241
        Width = 49
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 13
        Text = '30'
      end
      object Main_Control_Page_RichEdit: TRichEdit
        Left = 13
        Top = 340
        Width = 157
        Height = 301
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        Lines.Strings = (
          'Main_Control_Page_'
          'RichE'
          'di'
          't')
        ParentFont = False
        ScrollBars = ssVertical
        TabOrder = 14
      end
      object Pause_Edit: TEdit
        Left = 294
        Top = 118
        Width = 49
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 15
        Text = '10'
      end
      object Timestep_Edit: TEdit
        Left = 320
        Top = 93
        Width = 49
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 16
        Text = '0.02'
      end
      object CV_Mesh_Spacing_Edit: TEdit
        Left = 220
        Top = 241
        Width = 49
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 17
        Text = '2'
      end
      object CV_Vectors_Method_RadioGroup: TRadioGroup
        Left = 280
        Top = 164
        Width = 177
        Height = 61
        Caption = 'CV vectors method:'
        ItemIndex = 0
        Items.Strings = (
          'Based on local activations'
          'Just phase')
        TabOrder = 18
      end
      object Playback_Speed_Edit: TEdit
        Left = 236
        Top = 391
        Width = 65
        Height = 21
        TabOrder = 19
        Text = '2'
      end
      object Play_Button: TButton
        Left = 192
        Top = 360
        Width = 96
        Height = 25
        Caption = 'PLAY'
        TabOrder = 20
        OnClick = Play_ButtonClick
      end
      object Stop_Playback_Button: TButton
        Left = 294
        Top = 360
        Width = 96
        Height = 25
        Caption = 'STOP'
        TabOrder = 21
        OnClick = Stop_Playback_ButtonClick
      end
    end
    object TabSheet5: TTabSheet
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'Signals tab'
      ImageIndex = 5
      object Label9: TLabel
        Left = 18
        Top = 14
        Width = 115
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Grid electrodes spacing:'
      end
      object Label1: TLabel
        Left = 11
        Top = 60
        Width = 42
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Sampling'
      end
      object Label52: TLabel
        Left = 139
        Top = 34
        Width = 78
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Vertical spacing:'
      end
      object Label53: TLabel
        Left = 558
        Top = 12
        Width = 108
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'CL sliding window size:'
      end
      object Label54: TLabel
        Left = 558
        Top = 32
        Width = 111
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'CL sliding window step:'
      end
      object Label55: TLabel
        Left = 576
        Top = 54
        Width = 87
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Cycle length [ms]:'
      end
      object Signals_Tab_Chart: TChart
        Left = 2
        Top = 132
        Width = 471
        Height = 316
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Legend.Visible = False
        Title.Text.Strings = (
          'TChart')
        BottomAxis.Grid.Visible = False
        BottomAxis.Title.Caption = 'Time [ms]'
        LeftAxis.Grid.Visible = False
        LeftAxis.MinorTickCount = 0
        LeftAxis.TickLength = 0
        LeftAxis.Title.Caption = 'Voltage'
        View3D = False
        Color = clWhite
        TabOrder = 0
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object Series4: TFastLineSeries
          HoverElement = []
          SeriesColor = -1
          LinePen.Color = -1
          LinePen.Width = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series5: TFastLineSeries
          HoverElement = []
          SeriesColor = clBlack
          LinePen.Width = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series6: TFastLineSeries
          HoverElement = []
          SeriesColor = clBlack
          LinePen.Width = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series7: TFastLineSeries
          HoverElement = []
          SeriesColor = clBlack
          LinePen.Width = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series8: TFastLineSeries
          HoverElement = []
          SeriesColor = clBlack
          LinePen.Width = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series9: TFastLineSeries
          HoverElement = []
          LinePen.Color = 10708548
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series10: TFastLineSeries
          HoverElement = []
          LinePen.Color = 10708548
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series11: TFastLineSeries
          HoverElement = []
          LinePen.Color = 10708548
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series12: TFastLineSeries
          HoverElement = []
          LinePen.Color = 10708548
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series13: TFastLineSeries
          HoverElement = []
          LinePen.Color = 10708548
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series14: TFastLineSeries
          HoverElement = []
          LinePen.Color = 10708548
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series15: TFastLineSeries
          HoverElement = []
          LinePen.Color = 10708548
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series16: TFastLineSeries
          HoverElement = []
          LinePen.Color = 10708548
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series17: TFastLineSeries
          HoverElement = []
          LinePen.Color = 10708548
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series18: TFastLineSeries
          HoverElement = []
          LinePen.Color = 10708548
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series19: TFastLineSeries
          HoverElement = []
          LinePen.Color = 10708548
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series20: TFastLineSeries
          HoverElement = []
          LinePen.Color = 10708548
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series21: TFastLineSeries
          HoverElement = []
          LinePen.Color = 10708548
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series22: TFastLineSeries
          HoverElement = []
          LinePen.Color = 10708548
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series23: TFastLineSeries
          HoverElement = []
          LinePen.Color = 10708548
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series24: TFastLineSeries
          HoverElement = []
          LinePen.Color = 10708548
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series25: TFastLineSeries
          HoverElement = []
          LinePen.Color = 10708548
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series26: TFastLineSeries
          HoverElement = []
          LinePen.Color = 10708548
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series27: TFastLineSeries
          HoverElement = []
          LinePen.Color = 10708548
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series28: TFastLineSeries
          HoverElement = []
          LinePen.Color = 10708548
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series29: TFastLineSeries
          HoverElement = []
          LinePen.Color = 10708548
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series30: TFastLineSeries
          HoverElement = []
          LinePen.Color = 10708548
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series31: TFastLineSeries
          HoverElement = []
          LinePen.Color = 10708548
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series32: TFastLineSeries
          HoverElement = []
          LinePen.Color = 10708548
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series33: TFastLineSeries
          HoverElement = []
          LinePen.Color = 10708548
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series34: TFastLineSeries
          HoverElement = []
          LinePen.Color = 10708548
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series40: TLineSeries
          HoverElement = [heCurrent]
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series41: TLineSeries
          HoverElement = [heCurrent]
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series42: TLineSeries
          HoverElement = [heCurrent]
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series43: TLineSeries
          HoverElement = [heCurrent]
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series44: TLineSeries
          HoverElement = [heCurrent]
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series45: TLineSeries
          HoverElement = [heCurrent]
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series46: TLineSeries
          HoverElement = [heCurrent]
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series47: TLineSeries
          HoverElement = [heCurrent]
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series48: TLineSeries
          HoverElement = [heCurrent]
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series49: TLineSeries
          HoverElement = [heCurrent]
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series50: TLineSeries
          HoverElement = [heCurrent]
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series51: TLineSeries
          HoverElement = [heCurrent]
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series52: TLineSeries
          HoverElement = [heCurrent]
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series53: TLineSeries
          HoverElement = [heCurrent]
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series54: TLineSeries
          HoverElement = [heCurrent]
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series55: TLineSeries
          HoverElement = [heCurrent]
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series56: TLineSeries
          HoverElement = [heCurrent]
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series57: TLineSeries
          HoverElement = [heCurrent]
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series58: TLineSeries
          HoverElement = [heCurrent]
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series59: TLineSeries
          HoverElement = [heCurrent]
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series60: TLineSeries
          HoverElement = [heCurrent]
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series61: TLineSeries
          HoverElement = [heCurrent]
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series62: TLineSeries
          HoverElement = [heCurrent]
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series63: TLineSeries
          HoverElement = [heCurrent]
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series64: TLineSeries
          HoverElement = [heCurrent]
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series65: TLineSeries
          HoverElement = [heCurrent]
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series67: TLineSeries
          HoverElement = [heCurrent]
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series68: TLineSeries
          HoverElement = [heCurrent]
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series70: TLineSeries
          HoverElement = [heCurrent]
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series72: TPointSeries
          HoverElement = [heCurrent]
          ClickableLine = False
          Pointer.Brush.Color = clRed
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clRed
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series73: TPointSeries
          HoverElement = [heCurrent]
          ClickableLine = False
          Pointer.Brush.Color = clRed
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clRed
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series74: TPointSeries
          HoverElement = [heCurrent]
          ClickableLine = False
          Pointer.Brush.Color = clRed
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clRed
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series75: TPointSeries
          HoverElement = [heCurrent]
          ClickableLine = False
          Pointer.Brush.Color = clRed
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clRed
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series76: TPointSeries
          HoverElement = [heCurrent]
          ClickableLine = False
          Pointer.Brush.Color = clRed
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clRed
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series77: TPointSeries
          HoverElement = [heCurrent]
          ClickableLine = False
          Pointer.Brush.Color = clRed
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clRed
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series78: TPointSeries
          HoverElement = [heCurrent]
          ClickableLine = False
          Pointer.Brush.Color = clRed
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clRed
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series79: TPointSeries
          HoverElement = [heCurrent]
          ClickableLine = False
          Pointer.Brush.Color = clRed
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clRed
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series80: TPointSeries
          HoverElement = [heCurrent]
          ClickableLine = False
          Pointer.Brush.Color = clRed
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clRed
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series81: TPointSeries
          HoverElement = [heCurrent]
          ClickableLine = False
          Pointer.Brush.Color = clRed
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clRed
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series82: TPointSeries
          HoverElement = [heCurrent]
          ClickableLine = False
          Pointer.Brush.Color = clRed
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clRed
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series83: TPointSeries
          HoverElement = [heCurrent]
          ClickableLine = False
          Pointer.Brush.Color = clRed
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clRed
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series84: TPointSeries
          HoverElement = [heCurrent]
          ClickableLine = False
          Pointer.Brush.Color = clRed
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clRed
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series85: TPointSeries
          HoverElement = [heCurrent]
          ClickableLine = False
          Pointer.Brush.Color = clRed
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clRed
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series86: TPointSeries
          HoverElement = [heCurrent]
          ClickableLine = False
          Pointer.Brush.Color = clRed
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clRed
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series87: TPointSeries
          HoverElement = [heCurrent]
          ClickableLine = False
          Pointer.Brush.Color = clRed
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clRed
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series88: TPointSeries
          HoverElement = [heCurrent]
          ClickableLine = False
          Pointer.Brush.Color = clRed
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clRed
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series89: TPointSeries
          HoverElement = [heCurrent]
          ClickableLine = False
          Pointer.Brush.Color = clRed
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clRed
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series90: TPointSeries
          HoverElement = [heCurrent]
          ClickableLine = False
          Pointer.Brush.Color = clRed
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clRed
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series91: TPointSeries
          HoverElement = [heCurrent]
          ClickableLine = False
          Pointer.Brush.Color = clRed
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clRed
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series92: TPointSeries
          HoverElement = [heCurrent]
          ClickableLine = False
          Pointer.Brush.Color = clRed
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clRed
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series93: TPointSeries
          HoverElement = [heCurrent]
          ClickableLine = False
          Pointer.Brush.Color = clRed
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clRed
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series94: TPointSeries
          HoverElement = [heCurrent]
          ClickableLine = False
          Pointer.Brush.Color = clRed
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clRed
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series95: TPointSeries
          HoverElement = [heCurrent]
          ClickableLine = False
          Pointer.Brush.Color = clRed
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clRed
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series96: TPointSeries
          HoverElement = [heCurrent]
          ClickableLine = False
          Pointer.Brush.Color = clRed
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clRed
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series97: TPointSeries
          HoverElement = [heCurrent]
          ClickableLine = False
          Pointer.Brush.Color = clRed
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clRed
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series98: TPointSeries
          HoverElement = [heCurrent]
          ClickableLine = False
          Pointer.Brush.Color = clRed
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clRed
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series99: TPointSeries
          HoverElement = [heCurrent]
          ClickableLine = False
          Pointer.Brush.Color = clRed
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clRed
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series100: TPointSeries
          HoverElement = [heCurrent]
          ClickableLine = False
          Pointer.Brush.Color = clRed
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clRed
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series101: TPointSeries
          HoverElement = [heCurrent]
          ClickableLine = False
          Pointer.Brush.Color = clRed
          Pointer.HorizSize = 2
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clRed
          Pointer.Style = psRectangle
          Pointer.VertSize = 2
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object Signals_Tab_Refresh_Button: TButton
        Left = 220
        Top = 2
        Width = 52
        Height = 23
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Refresh'
        TabOrder = 1
        OnClick = Signals_Tab_Refresh_ButtonClick
      end
      object Grid_Spacing_Edit: TEdit
        Left = 139
        Top = 9
        Width = 37
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 2
        Text = '4'
      end
      object SC_RadioGroup: TRadioGroup
        Left = 416
        Top = -2
        Width = 126
        Height = 80
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Displayed'
        ItemIndex = 0
        Items.Strings = (
          'Signal'
          'Phase'
          'Signal+Phase'
          'Arrows')
        TabOrder = 3
        OnClick = SC_RadioGroupClick
      end
      object Record_Potentials_CheckBox: TCheckBox
        Left = 11
        Top = 34
        Width = 124
        Height = 19
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Record potentials'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 4
      end
      object Sampling_Edit: TEdit
        Left = 57
        Top = 57
        Width = 40
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 5
        Text = '0.25'
      end
      object Clear_Sig_History_Button: TButton
        Left = 102
        Top = 58
        Width = 51
        Height = 18
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Clear'
        TabOrder = 6
        OnClick = Clear_Sig_History_ButtonClick
      end
      object VS_Gap_Edit: TEdit
        Left = 221
        Top = 29
        Width = 37
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 7
        Text = '1'
      end
      object Signal_Type_To_Record_RadioGroup: TRadioGroup
        Left = 276
        Top = -2
        Width = 136
        Height = 71
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Signal to record'
        ItemIndex = 0
        Items.Strings = (
          'Voltage'
          'Unipolar electrogram'
          'Bipolar electrogram')
        TabOrder = 8
      end
      object CL_Chart: TChart
        Left = 315
        Top = 520
        Width = 400
        Height = 250
        Legend.Visible = False
        Title.Text.Strings = (
          'Correlation length')
        View3D = False
        TabOrder = 9
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object Series38: TLineSeries
          HoverElement = [heCurrent]
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series39: TLineSeries
          HoverElement = [heCurrent]
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object CL_W_Size_Edit: TEdit
        Left = 676
        Top = 9
        Width = 37
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 10
        Text = '200'
      end
      object CL_W_Step_Edit: TEdit
        Left = 676
        Top = 32
        Width = 37
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 11
        Text = '5'
      end
      object Corr_L_CheckBox: TCheckBox
        Left = 14
        Top = 82
        Width = 169
        Height = 17
        Caption = 'Automatic realculation of Corr L'
        TabOrder = 12
      end
      object Recalculate_CL_Button: TButton
        Left = 391
        Top = 110
        Width = 91
        Height = 18
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Recalculate CL'
        TabOrder = 13
        OnClick = Recalculate_CL_ButtonClick
      end
      object Basic_CL_Edit: TEdit
        Left = 675
        Top = 54
        Width = 37
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 14
        Text = '5'
      end
      object Calculation_Target_RadioGroup: TRadioGroup
        Left = 192
        Top = 73
        Width = 195
        Height = 55
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Calculate:'
        ItemIndex = 0
        Items.Strings = (
          'Mean phase coherence'
          'Correlation length')
        TabOrder = 15
        OnClick = Calculation_Target_RadioGroupClick
      end
      object MPC_Dist_Chart: TChart
        Left = 18
        Top = 520
        Width = 261
        Height = 250
        Legend.Visible = False
        Title.Text.Strings = (
          'Angle/MPC vs. Dist')
        BottomAxis.Title.Caption = 'Distnance'
        LeftAxis.Title.Caption = 'MPC/Corr'
        View3D = False
        PopupMenu = PopupMenu_mpc_vs_dist
        TabOrder = 16
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object LineSeries3: TPointSeries
          HoverElement = [heCurrent]
          ClickableLine = False
          Pointer.HorizSize = 1
          Pointer.InflateMargins = True
          Pointer.Pen.Visible = False
          Pointer.Style = psRectangle
          Pointer.VertSize = 1
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object LineSeries4: TLineSeries
          HoverElement = [heCurrent]
          SeriesColor = clRed
          Brush.BackColor = clDefault
          LinePen.Width = 3
          Pointer.HorizSize = 1
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.VertSize = 1
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object Angle_vs_distance_Button: TButton
        Left = 486
        Top = 110
        Width = 171
        Height = 18
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Get angle vs. distance scatter'
        TabOrder = 17
        OnClick = Angle_vs_distance_ButtonClick
      end
      object Recalculate_Phase_Button: TButton
        Left = 391
        Top = 88
        Width = 114
        Height = 18
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Recalculate phase'
        TabOrder = 18
        OnClick = Recalculate_Phase_ButtonClick
      end
      object Phase_vs_Distance_Source_RadioGroup: TRadioGroup
        Left = 717
        Top = 2
        Width = 195
        Height = 75
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'phase vs. distance source'
        ItemIndex = 2
        Items.Strings = (
          'CV vector orientation'
          'activation phase'
          'MPC between phase signals')
        TabOrder = 19
      end
      object Corr_Dist_Scatter_Type_RadioGroup: TRadioGroup
        Left = 677
        Top = 81
        Width = 120
        Height = 59
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'normal or loglog'
        ItemIndex = 0
        Items.Strings = (
          'normal'
          'log-log')
        TabOrder = 20
      end
      object Calculate_MPC_Button: TButton
        Left = 519
        Top = 88
        Width = 114
        Height = 18
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Calculate MPC'
        TabOrder = 21
      end
    end
    object TabSheet2: TTabSheet
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'Display'
      ImageIndex = 1
      object Z_Slice_Label: TLabel
        Left = 307
        Top = 348
        Width = 36
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Z slice: '
      end
      object T_Slice_Label: TLabel
        Left = 307
        Top = 397
        Width = 33
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'T slice:'
      end
      object Label28: TLabel
        Left = 294
        Top = 68
        Width = 16
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Min'
      end
      object Label29: TLabel
        Left = 294
        Top = 90
        Width = 20
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Max'
      end
      object Label8: TLabel
        Left = 448
        Top = 16
        Width = 54
        Height = 13
        Caption = 'Arrow size:'
      end
      object Label51: TLabel
        Left = 439
        Top = 51
        Width = 146
        Height = 13
        Caption = 'Arrow density (mesh spacing):'
      end
      object Display_Type_RadioGroup: TRadioGroup
        Left = 11
        Top = 2
        Width = 94
        Height = 53
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Geo type'
        ItemIndex = 0
        Items.Strings = (
          '2D slice/2 layers'
          '3D volume')
        TabOrder = 0
        OnClick = Display_Type_RadioGroupClick
      end
      object Variables_Plot_Chart: TChart
        Left = 298
        Top = 208
        Width = 274
        Height = 127
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Legend.Visible = False
        Title.Text.Strings = (
          '')
        View3D = False
        TabOrder = 1
        DefaultCanvas = 'TGDIPlusCanvas'
        PrintMargins = (
          15
          27
          15
          27)
        ColorPaletteIndex = 13
        object Series1: TFastLineSeries
          HoverElement = []
          LinePen.Color = 10708548
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series2: TFastLineSeries
          HoverElement = []
          LinePen.Color = 3513587
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object Plot_Variables_CheckBox: TCheckBox
        Left = 2
        Top = 208
        Width = 110
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Plot variables'
        TabOrder = 2
      end
      object Clear_Variables_Plot_Button: TButton
        Left = 576
        Top = 284
        Width = 51
        Height = 17
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Clear'
        TabOrder = 3
        OnClick = Clear_Variables_Plot_ButtonClick
      end
      object Z_Slice_TrackBar: TTrackBar
        Left = 301
        Top = 365
        Width = 204
        Height = 31
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Max = 100
        TabOrder = 4
        OnChange = Z_Slice_TrackBarChange
      end
      object T_Slice_TrackBar: TTrackBar
        Left = 301
        Top = 414
        Width = 204
        Height = 31
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Max = 100
        TabOrder = 5
        OnChange = T_Slice_TrackBarChange
      end
      object Export_Sig_Plot_Button: TButton
        Left = 631
        Top = 284
        Width = 51
        Height = 17
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Export'
        TabOrder = 6
        OnClick = Export_Sig_Plot_ButtonClick
      end
      object Display_Value_RadioGroup: TRadioGroup
        Left = 109
        Top = 2
        Width = 164
        Height = 333
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Display type'
        ItemIndex = 0
        Items.Strings = (
          'Voltage'
          'Coupling DX'
          'Coupling DY'
          'mi1'
          'mi2'
          'Custom Value'
          'Betw layer coupling'
          'History'
          'Phase map'
          'MPC (grid based)'
          'Level-set contours (f,g=0)')
        TabOrder = 7
        OnClick = Display_Value_RadioGroupClick
      end
      object Display_Sim_CheckBox: TCheckBox
        Left = 293
        Top = 5
        Width = 117
        Height = 12
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Display simulation'
        Checked = True
        State = cbChecked
        TabOrder = 8
      end
      object Plot_Source_RadioGroup: TRadioGroup
        Left = 568
        Top = 208
        Width = 126
        Height = 72
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Plot_Source_RadioGroup'
        ItemIndex = 0
        Items.Strings = (
          'Voltage+Current'
          'Total excited area')
        TabOrder = 9
      end
      object Min_Disp_Val_Edit: TEdit
        Left = 315
        Top = 66
        Width = 28
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 10
        Text = '0.5'
      end
      object Max_Disp_Val_Edit: TEdit
        Left = 315
        Top = 88
        Width = 28
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 11
        Text = '1'
      end
      object Set_Custom_Color_Range_Button: TButton
        Left = 348
        Top = 89
        Width = 51
        Height = 17
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Set'
        TabOrder = 12
        OnClick = Set_Custom_Color_Range_ButtonClick
      end
      object Display_Conduction_Arrows_Mean_CheckBox: TCheckBox
        Left = 293
        Top = 29
        Width = 117
        Height = 12
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Display simulation'
        Checked = True
        State = cbChecked
        TabOrder = 13
      end
      object Arrow_Size_Edit: TEdit
        Left = 516
        Top = 13
        Width = 53
        Height = 21
        TabOrder = 14
        Text = '20'
      end
      object Arrow_Mesh_Spacing_Edit: TEdit
        Left = 532
        Top = 70
        Width = 53
        Height = 21
        TabOrder = 15
        Text = '5'
      end
    end
    object TabSheet3: TTabSheet
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'ISIs'
      ImageIndex = 2
      object Label27: TLabel
        Left = 296
        Top = 5
        Width = 18
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'BCL'
      end
      object Value_Label: TLabel
        Left = 372
        Top = 5
        Width = 30
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Value:'
      end
      object Signal_Display_ScrollBar: TScrollBar
        Left = 2
        Top = 319
        Width = 302
        Height = 16
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        PageSize = 0
        TabOrder = 0
      end
      object Single_Signal_Chart: TChart
        Left = 2
        Top = 25
        Width = 274
        Height = 143
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Legend.Visible = False
        Title.Text.Strings = (
          'Voltage')
        View3D = False
        TabOrder = 1
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object Series3: TLineSeries
          HoverElement = [heCurrent]
          SeriesColor = -1
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series35: TLineSeries
          HoverElement = [heCurrent]
          SeriesColor = 33023
          Brush.BackColor = clDefault
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object ISIs_Chart: TChart
        Left = 2
        Top = 172
        Width = 274
        Height = 143
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Legend.Visible = False
        Title.Text.Strings = (
          'Inter Spike Intervals (ISI)')
        View3D = False
        TabOrder = 2
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object LineSeries1: TLineSeries
          HoverElement = [heCurrent]
          SeriesColor = -1
          Brush.BackColor = clDefault
          Pointer.Brush.Gradient.EndColor = -1
          Pointer.Gradient.EndColor = -1
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = True
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series37: TPointSeries
          HoverElement = [heCurrent]
          SeriesColor = clRed
          ClickableLine = False
          Pointer.Brush.Gradient.EndColor = clRed
          Pointer.Gradient.EndColor = clRed
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object Paramter_Selection_RadioGroup: TRadioGroup
        Left = 2
        Top = 339
        Width = 198
        Height = 72
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Parameter selection'
        Columns = 2
        Items.Strings = (
          'Mean'
          'SD'
          'Porta'#39's Index'
          'Voltage cross-corr')
        TabOrder = 3
        OnClick = Paramter_Selection_RadioGroupClick
      end
      object ISIs_Parameters_Chart: TChart
        Left = 2
        Top = 415
        Width = 274
        Height = 143
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Legend.Visible = False
        Title.Text.Strings = (
          '')
        View3D = False
        TabOrder = 4
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object LineSeries2: TLineSeries
          HoverElement = [heCurrent]
          SeriesColor = -1
          Brush.BackColor = clDefault
          Pointer.Brush.Gradient.EndColor = -1
          Pointer.Gradient.EndColor = -1
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = True
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object Update_Par_Button: TButton
        Left = 204
        Top = 345
        Width = 51
        Height = 39
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Update'
        TabOrder = 5
        OnClick = Update_Par_ButtonClick
      end
      object BCL_Edit: TEdit
        Left = 318
        Top = 2
        Width = 40
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 6
        Text = '50'
      end
    end
    object TabSheet4: TTabSheet
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'Modulation'
      ImageIndex = 3
      object Label2: TLabel
        Left = 55
        Top = 16
        Width = 33
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Length'
      end
      object Label3: TLabel
        Left = 38
        Top = 34
        Width = 47
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Amplitude'
      end
      object Label4: TLabel
        Left = 12
        Top = 54
        Width = 76
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Assymetry ratio'
      end
      object Label5: TLabel
        Left = 2
        Top = 73
        Width = 93
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Region relative size'
      end
      object Label12: TLabel
        Left = 55
        Top = 118
        Width = 47
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Amplitude'
      end
      object Label13: TLabel
        Left = 47
        Top = 135
        Width = 60
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Pperiod [ms]'
      end
      object Label19: TLabel
        Left = 55
        Top = 175
        Width = 57
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'mi1 left side'
      end
      object Label20: TLabel
        Left = 55
        Top = 192
        Width = 57
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'mi2 left side'
      end
      object Label21: TLabel
        Left = 55
        Top = 216
        Width = 63
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'mi1 right side'
      end
      object Label22: TLabel
        Left = 55
        Top = 235
        Width = 63
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'mi2 right side'
      end
      object Label23: TLabel
        Left = 36
        Top = 255
        Width = 86
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Ratio where cut is'
      end
      object Label24: TLabel
        Left = 423
        Top = 41
        Width = 32
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Radius'
      end
      object Label25: TLabel
        Left = 421
        Top = 62
        Width = 37
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Number'
      end
      object Label26: TLabel
        Left = 372
        Top = 16
        Width = 186
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Random disks (of breakup parameters)'
      end
      object Label30: TLabel
        Left = 12
        Top = 312
        Width = 186
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'INHOMOGENEOUS LANDSCAPE (I,R,p)'
      end
      object Label31: TLabel
        Left = 28
        Top = 402
        Width = 32
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Radius'
      end
      object Label32: TLabel
        Left = 9
        Top = 423
        Width = 50
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'probability'
      end
      object Label33: TLabel
        Left = 31
        Top = 447
        Width = 29
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Depth'
      end
      object Label34: TLabel
        Left = 136
        Top = 399
        Width = 16
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'min'
      end
      object Label35: TLabel
        Left = 134
        Top = 416
        Width = 20
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'max'
      end
      object Label59: TLabel
        Left = 29
        Top = 536
        Width = 54
        Height = 13
        Caption = 'period [ms]'
      end
      object Asm_Len_Edit: TEdit
        Left = 92
        Top = 11
        Width = 40
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 0
        Text = '10'
      end
      object Asm_Amp_Edit: TEdit
        Left = 92
        Top = 31
        Width = 40
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 1
        Text = '0.0005'
      end
      object Ass_Ratio_Edit: TEdit
        Left = 92
        Top = 51
        Width = 40
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 2
        Text = '0.2'
      end
      object Add_Assymetric_Modulation_CheckBox: TCheckBox
        Left = 146
        Top = 14
        Width = 222
        Height = 12
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Assymetric triangular voltage modulation'
        TabOrder = 3
        OnClick = Add_Assymetric_Modulation_CheckBoxClick
      end
      object Ass_Region_Size_Edit: TEdit
        Left = 99
        Top = 71
        Width = 41
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 4
        Text = '0.5'
      end
      object CMA_Edit: TEdit
        Left = 114
        Top = 113
        Width = 40
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 5
        Text = '0.0001'
      end
      object CML_Edit: TEdit
        Left = 114
        Top = 135
        Width = 41
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 6
        Text = '40'
      end
      object Add_Current_Modulation_CheckBox: TCheckBox
        Left = 169
        Top = 116
        Width = 209
        Height = 12
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'abs(sin(om*T)) voltage modulation'
        TabOrder = 7
        OnClick = Add_Current_Modulation_CheckBoxClick
      end
      object mi1_left_Edit: TEdit
        Left = 125
        Top = 173
        Width = 40
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 8
        Text = '0.2'
      end
      object mi2_left_Edit: TEdit
        Left = 125
        Top = 193
        Width = 41
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 9
        Text = '0.2'
      end
      object mi1_right_Edit: TEdit
        Left = 126
        Top = 213
        Width = 40
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 10
        Text = '0.2'
      end
      object mi2_right_Edit: TEdit
        Left = 126
        Top = 233
        Width = 40
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 11
        Text = '0.2'
      end
      object SET_MI_Button: TButton
        Left = 170
        Top = 235
        Width = 56
        Height = 38
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'SET'
        TabOrder = 12
        OnClick = SET_MI_ButtonClick
      end
      object Cut_ratio_mi_Edit: TEdit
        Left = 126
        Top = 252
        Width = 40
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 13
        Text = '0.5'
      end
      object DR_Edit: TEdit
        Left = 464
        Top = 34
        Width = 40
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 14
        Text = '10'
      end
      object DN_Edit: TEdit
        Left = 464
        Top = 56
        Width = 40
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 15
        Text = '4'
      end
      object Generate_Random_Breakup_Rings_Button: TButton
        Left = 508
        Top = 44
        Width = 52
        Height = 31
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Generate'
        TabOrder = 16
        OnClick = Generate_Random_Breakup_Rings_ButtonClick
      end
      object RL_Edit: TEdit
        Left = 64
        Top = 400
        Width = 41
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 17
        Text = '10'
      end
      object ppL_Edit: TEdit
        Left = 64
        Top = 423
        Width = 41
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 18
        Text = '0.01'
      end
      object Landscape_mi1_Button: TButton
        Left = 109
        Top = 438
        Width = 90
        Height = 31
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Generate'
        TabOrder = 19
        OnClick = Landscape_mi1_ButtonClick
      end
      object IL_Edit: TEdit
        Left = 64
        Top = 445
        Width = 41
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 20
        Text = '0.04'
      end
      object mi_min_Edit: TEdit
        Left = 159
        Top = 396
        Width = 40
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 21
        Text = '0.02'
      end
      object mi_max_Edit: TEdit
        Left = 159
        Top = 415
        Width = 40
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 22
        Text = '0.2'
      end
      object Gen2_Button: TButton
        Left = 564
        Top = 34
        Width = 88
        Height = 41
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Generate inversed'
        TabOrder = 23
        OnClick = Gen2_ButtonClick
      end
      object InhVar_RadioGroup: TRadioGroup
        Left = 12
        Top = 326
        Width = 350
        Height = 61
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Variable'
        ItemIndex = 0
        Items.Strings = (
          'Coupling (D)'
          'mi1'
          'mi2')
        TabOrder = 24
        OnClick = InhVar_RadioGroupClick
      end
      object Mimi2_Disk_Button: TButton
        Left = 213
        Top = 397
        Width = 154
        Height = 22
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Centered disk (m1,mi2,R)'
        TabOrder = 25
        OnClick = Mimi2_Disk_ButtonClick
      end
      object Ectopy_In_Middle_CheckBox: TCheckBox
        Left = 32
        Top = 512
        Width = 194
        Height = 17
        Caption = 'Ectopy in the middle'
        Checked = True
        State = cbChecked
        TabOrder = 26
      end
      object Ectopy_CL_ms_Edit: TEdit
        Left = 96
        Top = 536
        Width = 59
        Height = 21
        TabOrder = 27
        Text = '40'
      end
    end
    object Coupling: TTabSheet
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'Coupling'
      ImageIndex = 4
      object Label6: TLabel
        Left = 14
        Top = 66
        Width = 37
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Number'
      end
      object Label7: TLabel
        Left = 16
        Top = 88
        Width = 32
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Radius'
      end
      object Label10: TLabel
        Left = 16
        Top = 137
        Width = 37
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Number'
      end
      object Label11: TLabel
        Left = 10
        Top = 157
        Width = 44
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = '% length'
      end
      object Label14: TLabel
        Left = 2
        Top = 8
        Width = 165
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Initial diffusion coef (X,Y direction)'
      end
      object Label15: TLabel
        Left = 114
        Top = 83
        Width = 28
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Width'
      end
      object Label16: TLabel
        Left = 114
        Top = 103
        Width = 84
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = '% of max D more'
      end
      object Label17: TLabel
        Left = 128
        Top = 154
        Width = 32
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Radius'
      end
      object Label18: TLabel
        Left = 10
        Top = 177
        Width = 34
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = '% of D'
      end
      object Label37: TLabel
        Left = 128
        Top = 187
        Width = 26
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Min D'
      end
      object Label38: TLabel
        Left = 128
        Top = 207
        Width = 30
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Max D'
      end
      object Label39: TLabel
        Left = 2
        Top = 249
        Width = 119
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'INTER-LAYER COUPLING'
      end
      object Label40: TLabel
        Left = 9
        Top = 272
        Width = 58
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Min coupling'
      end
      object Label41: TLabel
        Left = 8
        Top = 289
        Width = 62
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Max coupling'
      end
      object Label42: TLabel
        Left = 201
        Top = 268
        Width = 37
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Number'
      end
      object Label43: TLabel
        Left = 203
        Top = 291
        Width = 32
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Radius'
      end
      object Label44: TLabel
        Left = 114
        Top = 268
        Width = 79
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Disk connections'
      end
      object NDiffv1_Edit: TEdit
        Left = 57
        Top = 64
        Width = 31
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 0
        Text = '30'
      end
      object DRadius_Edit: TEdit
        Left = 57
        Top = 86
        Width = 31
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 1
        Text = '10'
      end
      object Generate_Coupling_v1_Button: TButton
        Left = 251
        Top = 7
        Width = 62
        Height = 40
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'UPDATE'
        TabOrder = 2
        OnClick = Generate_Coupling_v1_ButtonClick
      end
      object Lines_No_Edit: TEdit
        Left = 59
        Top = 135
        Width = 29
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 3
        Text = '5'
      end
      object Perc_Len_Edit: TEdit
        Left = 59
        Top = 155
        Width = 29
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 4
        Text = '90'
      end
      object DX_Edit: TEdit
        Left = 174
        Top = 6
        Width = 35
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 5
        Text = '0.2'
      end
      object DY_Edit: TEdit
        Left = 213
        Top = 6
        Width = 34
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 6
        Text = '0.2'
      end
      object Disks_CheckBox: TCheckBox
        Left = 2
        Top = 38
        Width = 196
        Height = 22
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Small non-conducting discs'
        TabOrder = 7
      end
      object Lines_CheckBox: TCheckBox
        Left = 2
        Top = 115
        Width = 53
        Height = 18
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Lines'
        TabOrder = 8
      end
      object ARings_CheckBox: TCheckBox
        Left = 114
        Top = 64
        Width = 111
        Height = 15
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Add rings around'
        TabOrder = 9
      end
      object AR_Radius_Edit: TEdit
        Left = 205
        Top = 80
        Width = 31
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 10
        Text = '30'
      end
      object AR_Perc_Edit: TEdit
        Left = 205
        Top = 103
        Width = 31
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 11
        Text = '10'
      end
      object CDCB_CheckBox: TCheckBox
        Left = 114
        Top = 135
        Width = 124
        Height = 15
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Central disk'
        TabOrder = 12
      end
      object CD_R_Edit: TEdit
        Left = 170
        Top = 152
        Width = 31
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 13
        Text = '30'
      end
      object Reduce_Button: TButton
        Left = 205
        Top = 154
        Width = 52
        Height = 17
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Reduce'
        TabOrder = 14
        OnClick = Reduce_ButtonClick
      end
      object PL_Edit: TEdit
        Left = 59
        Top = 174
        Width = 29
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 15
        Text = '90'
      end
      object Min_Z_D_Edit: TEdit
        Left = 162
        Top = 185
        Width = 30
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 16
        Text = '0.15'
      end
      object Max_Z_D_Edit: TEdit
        Left = 162
        Top = 207
        Width = 30
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 17
        Text = '0.25'
      end
      object Z_D_Grad_Button: TButton
        Left = 196
        Top = 187
        Width = 72
        Height = 20
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Z axis D grad'
        TabOrder = 18
        OnClick = Z_D_Grad_ButtonClick
      end
      object mincp_Edit: TEdit
        Left = 75
        Top = 266
        Width = 29
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 19
        Text = '0'
      end
      object maxcp_Edit: TEdit
        Left = 75
        Top = 289
        Width = 29
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 20
        Text = '0.2'
      end
      object DCLN_Edit: TEdit
        Left = 244
        Top = 266
        Width = 31
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 21
        Text = '10'
      end
      object DCLR_Edit: TEdit
        Left = 244
        Top = 289
        Width = 31
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 22
        Text = '4'
      end
      object Set_Layers_Coupling_Button: TButton
        Left = 279
        Top = 267
        Width = 62
        Height = 40
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'UPDATE'
        TabOrder = 23
        OnClick = Set_Layers_Coupling_ButtonClick
      end
    end
    object TabSheet6: TTabSheet
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'ECG signal'
      ImageIndex = 6
      object Clear_ECG_Button: TButton
        Left = 127
        Top = 2
        Width = 52
        Height = 19
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Clear'
        TabOrder = 0
        OnClick = Clear_ECG_ButtonClick
      end
      object Record_ECG_CheckBox: TCheckBox
        Left = 35
        Top = 2
        Width = 81
        Height = 19
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Record ECG'
        TabOrder = 1
      end
      object ECG_Chart: TChart
        Left = 11
        Top = 36
        Width = 274
        Height = 140
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Legend.Visible = False
        Title.Text.Strings = (
          'ECG')
        BottomAxis.Grid.Visible = False
        LeftAxis.Grid.Visible = False
        View3D = False
        Color = clWhite
        TabOrder = 2
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object Series36: TLineSeries
          HoverElement = [heCurrent]
          SeriesColor = clBlack
          Brush.BackColor = clDefault
          Pointer.Brush.Gradient.EndColor = clBlack
          Pointer.Gradient.EndColor = clBlack
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
      object Export_Button: TButton
        Left = 203
        Top = 2
        Width = 51
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Export'
        TabOrder = 3
        OnClick = Export_ButtonClick
      end
    end
    object TabSheet7: TTabSheet
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = 'Critical Reg Protocol'
      ImageIndex = 7
      object Label45: TLabel
        Left = 38
        Top = 55
        Width = 181
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Depth level (2: r=50%,3: r=33%,...)'
      end
      object Label46: TLabel
        Left = 11
        Top = 82
        Width = 208
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Number of CRs (1: just one is searched,...)'
      end
      object Label47: TLabel
        Left = 11
        Top = 104
        Width = 3
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
      end
      object Label48: TLabel
        Left = 18
        Top = 118
        Width = 200
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Tested regions overlap during search [%]'
      end
      object Label49: TLabel
        Left = -8
        Top = 166
        Width = 220
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Simulation time to test for AF termination [ms]'
      end
      object CR_Progress_Label: TLabel
        Left = 285
        Top = 49
        Width = 60
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Iteration...'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label50: TLabel
        Left = 70
        Top = 27
        Width = 147
        Height = 13
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Number of tests per single CR:'
      end
      object CR_Level_ComboBox: TComboBox
        Left = 224
        Top = 53
        Width = 44
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Style = csDropDownList
        ItemIndex = 0
        TabOrder = 0
        Text = '2'
        OnChange = CR_Level_ComboBoxChange
        Items.Strings = (
          '2'
          '3'
          '4')
      end
      object CR_Number_ComboBox: TComboBox
        Left = 224
        Top = 80
        Width = 44
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Style = csDropDownList
        ItemIndex = 1
        TabOrder = 1
        Text = '2'
        OnChange = CR_Number_ComboBoxChange
        Items.Strings = (
          '1'
          '2'
          '3'
          '4')
      end
      object Diff_Sizes_CR_Search_CheckBox: TCheckBox
        Left = 38
        Top = 141
        Width = 223
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Search for CRs of different sizes?'
        TabOrder = 2
        OnClick = Diff_Sizes_CR_Search_CheckBoxClick
      end
      object Begin_CR_Protocol_Button: TButton
        Left = 285
        Top = 93
        Width = 92
        Height = 90
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'BEGIN'
        TabOrder = 3
        OnClick = Begin_CR_Protocol_ButtonClick
      end
      object CR_Regions_Overlap_ComboBox: TComboBox
        Left = 224
        Top = 113
        Width = 44
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Style = csDropDownList
        ItemIndex = 5
        TabOrder = 4
        Text = '50'
        OnChange = CR_Regions_Overlap_ComboBoxChange
        Items.Strings = (
          '0'
          '10'
          '20'
          '30'
          '40'
          '50'
          '60'
          '70'
          '80'
          '90')
      end
      object CRs_ListBox: TListBox
        Left = 11
        Top = 208
        Width = 99
        Height = 143
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        ItemHeight = 13
        TabOrder = 5
        OnClick = CRs_ListBoxClick
      end
      object CR_Test_Time_Edit: TEdit
        Left = 224
        Top = 166
        Width = 42
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 6
        Text = '500'
        OnChange = CR_Test_Time_EditChange
      end
      object TSTN_Edit: TEdit
        Left = 224
        Top = 25
        Width = 42
        Height = 21
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        TabOrder = 7
        Text = '3'
        OnChange = CR_Test_Time_EditChange
      end
      object Test_CR_Button1: TButton
        Left = 114
        Top = 208
        Width = 52
        Height = 34
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'Test CR'
        TabOrder = 8
        OnClick = Test_CR_Button1Click
      end
      object Stop2_Button: TButton
        Left = 114
        Top = 246
        Width = 52
        Height = 23
        Margins.Left = 2
        Margins.Top = 2
        Margins.Right = 2
        Margins.Bottom = 2
        Caption = 'STOP'
        TabOrder = 9
        OnClick = Stop_Sim_ButtonClick
      end
    end
  end
  object Data_FileListBox: TFileListBox
    Left = 32
    Top = 433
    Width = 99
    Height = 66
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    ItemHeight = 13
    TabOrder = 1
    Visible = False
  end
  object Panel_1: TPanel
    Left = 8
    Top = 29
    Width = 201
    Height = 164
    TabOrder = 2
    OnMouseDown = Panel_1MouseDown
    OnMouseMove = Panel_1MouseMove
    OnMouseUp = Panel_1MouseUp
  end
  object MainMenu1: TMainMenu
    Left = 136
    Top = 200
    object File1: TMenuItem
      Caption = 'File'
      object LoadData1: TMenuItem
        Caption = 'Load Data'
        OnClick = LoadData1Click
      end
      object SaveData1: TMenuItem
        Caption = 'Save Data'
        OnClick = SaveData1Click
      end
      object N4: TMenuItem
        Caption = '-'
      end
      object ExportVoltageHistorytextfile1: TMenuItem
        Caption = 'Export Voltage History (text file)'
        OnClick = ExportVoltageHistorytextfile1Click
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Exit1: TMenuItem
        Caption = 'Exit'
        OnClick = Exit1Click
      end
    end
    object Debug1: TMenuItem
      Caption = 'Debug'
      object Displaymodulation1: TMenuItem
        Caption = 'Display modulation'
        OnClick = Displaymodulation1Click
      end
    end
    object SIMULATIONTEMPLATES1: TMenuItem
      Caption = 'SIMULATION TEMPLATES'
      object Gettotalcrosscorelationpowerinfnofprobofablationcouplingv11: TMenuItem
        Caption = 
          'Get total crosscorelation power in f-n of prob of ablation (coup' +
          'ling v1)'
        OnClick = Gettotalcrosscorelationpowerinfnofprobofablationcouplingv11Click
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object CriticalRegions1: TMenuItem
        Caption = 'Critical Regions'
        object GeneratesetofstudieswihtstableAF1: TMenuItem
          Caption = '1. Generate a set of studies with stable AF'
          OnClick = GeneratesetofstudieswihtstableAF1Click
        end
        object ProcessfolderandcomputeCRs1: TMenuItem
          Caption = '2. Process folder and compute CRs'
          OnClick = ProcessfolderandcomputeCRs1Click
        end
        object N3Computeunipolarelectrograms1: TMenuItem
          Caption = '3. Compute electrograms'
          OnClick = N3Computeunipolarelectrograms1Click
        end
        object N4Computeelectrogramparameters1: TMenuItem
          Caption = '4. Compute electrogram parameters'
          OnClick = N4Computeelectrogramparameters1Click
        end
      end
    end
    object Signalprocessing1: TMenuItem
      Caption = 'Signal processing'
      object ComputeMPC1: TMenuItem
        Caption = 'Compute MPC'
        OnClick = ComputeMPC1Click
      end
      object ComputeElectrogramParameters1: TMenuItem
        Caption = 'Compute mean cycle length'
        OnClick = ComputeElectrogramParameters1Click
      end
      object ComputeSDofcyclelength1: TMenuItem
        Caption = 'Compute SD of cycle length'
        OnClick = ComputeSDofcyclelength1Click
      end
      object Computecausalitymap1: TMenuItem
        Caption = 
          'Compute point2point causality map (with respect to last pointed ' +
          'point)'
        OnClick = Computecausalitymap1Click
      end
      object Computeglobalcausalitymaplongcomputations1: TMenuItem
        Caption = 'Compute global causality map (long computations)'
        OnClick = Computeglobalcausalitymaplongcomputations1Click
      end
    end
    object Options1: TMenuItem
      Caption = 'Options'
      object Options2: TMenuItem
        Caption = 'Options'
        OnClick = Options2Click
      end
    end
    object Debug2: TMenuItem
      Caption = 'Debug'
      object Causalitytest1: TMenuItem
        Caption = 'Causality test'
        OnClick = Causalitytest1Click
      end
    end
    object VIRTUALABLATIONS1: TMenuItem
      Caption = 'VIRTUAL ABLATIONS'
      object AblateaccordingtoCL1: TMenuItem
        Caption = 'Ablate fastest regions (CL based)'
        OnClick = AblateaccordingtoCL1Click
      end
    end
    object Anandextension1: TMenuItem
      Caption = 'Anand extension'
      object Generatesimulationfile1: TMenuItem
        Caption = 'Generate simulation file (from current time point)'
        OnClick = Generatesimulationfile1Click
      end
      object GeneratesimulationfilePLANARWAVES1: TMenuItem
        Caption = 'Generate simulation file (PLANAR WAVES)'
        OnClick = GeneratesimulationfilePLANARWAVES1Click
      end
    end
    object Help1: TMenuItem
      Caption = 'Help'
      object Debug3: TMenuItem
        Caption = 'About'
      end
      object N5: TMenuItem
        Caption = '-'
      end
      object Debug11: TMenuItem
        Caption = 'Debug1'
        OnClick = Debug11Click
      end
    end
  end
  object EP_Mapping_3D_Window_PopupMenu: TPopupMenu
    Left = 96
    Top = 328
    object Poiningmode1: TMenuItem
      Tag = 1
      Caption = 'Poining mode'
      OnClick = Poiningmode1Click
    end
    object Rotate2: TMenuItem
      Tag = 2
      Caption = 'Rotate'
      Checked = True
      OnClick = Rotate2Click
    end
    object Zoom2: TMenuItem
      Tag = 3
      Caption = 'Zoom'
      OnClick = Zoom2Click
    end
    object Pan2: TMenuItem
      Tag = 4
      Caption = 'Pan'
      OnClick = Pan2Click
    end
    object N3: TMenuItem
      Caption = '-'
    end
    object Pushas1stsignal1: TMenuItem
      Caption = 'Push as 1st signal'
      OnClick = Pushas1stsignal1Click
    end
    object Pushas2ndsignal1: TMenuItem
      Caption = 'Push as 2nd signal'
      OnClick = Pushas2ndsignal1Click
    end
  end
  object SaveDialog1: TSaveDialog
    Left = 112
    Top = 616
  end
  object OpenDialog1: TOpenDialog
    Left = 192
    Top = 480
  end
  object PopupMenu_mpc_vs_dist: TPopupMenu
    Left = 248
    Top = 880
    object SaveXYpointstocsvfile1: TMenuItem
      Caption = 'Save XY points to csv file'
      OnClick = SaveXYpointstocsvfile1Click
    end
  end
end
