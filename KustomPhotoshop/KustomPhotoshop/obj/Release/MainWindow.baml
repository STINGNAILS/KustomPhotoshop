   M S B A M L   `   `   `  ���� [  WSystem.Windows.Forms, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089Y@clr-namespace:System.Windows.Forms;assembly=System.Windows.FormsSystem.Windows.Forms  J FKustomPhotoshop, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null1clr-namespace:KustomPhotoshopKustomPhotoshop 1clr-namespace:KustomPhotoshopKustomPhotoshop     KustomPhotoshop.MainWindow   W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35U QWPFToolkit, Version=3.5.40128.1, Culture=neutral, PublicKeyToken=31bf3856ad364e35^ ZWindowsFormsIntegration, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35H 9http://schemas.microsoft.com/winfx/2006/xaml/presentation      R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     Owf@clr-namespace:System.Windows.Forms;assembly=System.Windows.Forms     @d2http://schemas.microsoft.com/expression/blend/2008     Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     X TXceed.Wpf.Toolkit, Version=3.0.0.0, Culture=neutral, PublicKeyToken=3e4669d2f30244f44xctk)http://schemas.xceed.com/wpf/xaml/toolkit  -	renderersclr-namespace:KustomPhotoshop  )localclr-namespace:KustomPhotoshop  -     �Name$  window�� � Title$ Kustom Photoshop��$	��564��$	��881�� � WindowStyle$ ThreeDBorderWindow=�.���  � RowDefinitions �� $��20 ��� $
��512* � � ColumnDefinitions �� $��64 ��� $��6* ��� $	��250 �.+�>� $��0�����	#FF404040$��2��7  1System.Windows.Forms.Integration.WindowsFormsHost  -   $��HostDirectX��$��1��$��1��   Child. $  KustomPhotoshop.DirectXControl  $  KustomPhotoshop.DirectXControl   	MouseDown$! DirectXRenderer_OnMouseDown��   	MouseMove$! DirectXRenderer_OnMouseMove��   
MouseWheel$" DirectXRenderer_OnMouseWheel��	   MouseUp$	 DirectXRenderer_OnMouseUp��
   SizeChanged$ 
 DirectXControl_SizeChanged��   Paint$ DirectXControl_Paint��-   directXControl��>� $��2�����	#FF404040���	#FF202020$��2q�$��2��]� ���	#FF202020$��3��.+��� o� -    o�Name$
 menu��$	��Top=����	#FF282828
����������	#FF282828.@�m� $��_File�����	#FF282828���	#FF282828.@�m� -   $
��_New�����	#FF282828
����������	#FF282828}�. m� -   $��_Open�����	#FF282828
����������	#FF282828�� ���	#FF080808$��0q�m� -   $��_Save�����	#FF282828
����������	#FF282800�� ���	#FF080808$��0q�m� -   $��_Exit�����	#FF282828
����������	#FF282828d� -	    d�Name$ polyLineButton��$
��Left=�$	��Top=�$��64��$��64��$��0,1,0,0q�
���   �$��1��$��0q����� .R���  �� Orientation$ 
Horizontal=�}�. $��64��.+��� $��Resources/PolyLine.png��$��0,0,0,0q�$��64��d� -
   $ triangleButton��$
��Left=�$	��Top=�$��64��$��64��$��0,65,0,0q�$��1�����	#00000000$��0q����� .R��� $ 
Horizontal=�}�. $��64��.+��� $��Resources/Triangle.png��$��0,0,0,0q�$��64��d� -   $ 
quadButton��$
��Left=�$	��Top=�$��64��$��64��
���   �$��	0,129,0,0q�$��1��$��0q����� .R��� $ 
Horizontal=�}�. $��64��.+��� $��Resources/Quad.png��$��0,0,0,0q�$��64��d� -   $ ellipseButton��$
��Left=�$	��Top=�$��64��$��64��
���   �$��	0,193,0,0q�$��1��$��0q����� .R��� $ 
Horizontal=�}�. $��64��.+��� $��Resources/Ellipse.png��$��0,0,0,0q�$��64���� -    ��Name$ foregroundButton��$
��Left=�$��8,0,0,64q�$��48��$��13(�$��48��$��Bottom=�$��1�� � ToolTip$ To the foreground��$��0q����� .R��� $ 
Horizontal=�}�. $��48��.+��� $!��Resources/Arrow Up - 08.png��$��0,0,0,0q�$��48���� -   $ deleteButton��$��Delete��$
��Left=�$��8,0,0,8q�$��48��$��48��$��Bottom=�$��1��$ Remove��$��0q����� .R��� $ 
Horizontal=�}�. $��48��.+��� $��Resources/Trash-01-WF.png��$��0,0,0,0q�$��48���� -    ��Name$ label1��$��Figure properties��$��2��$
��Left=�$	��Top=�$��Segoe UI Semibold)�$��16(�
�������$	��245��$��32��
�������$��2,2,0,0q�$��1���� -   $ label2��$��Figure color��$��2��$
��Left=�$��	30,32,0,0q�$	��Top=�$��16(�
�������$��1��#  Xceed.Wpf.Toolkit.ColorPicker  -     Name$ figureColorPicker��$��2��$
��Left=�$	��Top=�$	��100��$��22��$��
140,38,0,0q����	#FF404040$��1��   SelectedColor �� *CurrentColor+ �� Mode$ TwoWay=� �� UpdateSourceTrigger$ PropertyChanged=� �� ElementName$ window���� $��2��$
��Left=�$	��100��$��	10,16,0,0q�$	��Top=�$	��230��$��1���� -   $ label3��$��Border color��$��2��$
��Left=�$��	30,66,0,0q�$	��Top=�$��16(�
�������$��1��  -   $ borderColorPicker��$��2��$
��Left=�$	��Top=�$	��100��$��22��$��
140,72,0,0q����	#FF404040$��1�� �� *CurrentBorderColor+$ TwoWay=�$ PropertyChanged=�$ window���� $��2��$
��Left=�$	��100��$��	10,49,0,0q�$	��Top=�$	��230��$��1���� -   $ label4��$��Border width��$��2��$
��Left=�$��
30,100,0,0q�$	��Top=�$��16(�
�������$��1���� -   $ 
widthLabel��$
��1.00��$��2��$
��Left=�$��140,100,0,0q�$	��Top=�$��16(�
�������$��1���� -    ��Name$ widthSlider��$��2��$
��Left=�$��
35,140,0,0q�$	��Top=�$	��205�� �� IsMoveToPointEnabled . 
�������$��1�� � Value �� *CurrentBorderWidth+$ TwoWay=�$ PropertyChanged=�$ window���� -   $ 	viewLabel��$��View properties��$��2��$
��Left=�$��	2,170,0,0q�$	��Top=�$��Segoe UI Semibold)�$��16(�
�������$	��245��$��32��
�������$��1���� -   $ label5��$��Coordinates��$��2��$
��Left=�$��
30,202,0,0q�$	��Top=�$��16(�
�������$��1���� -   $ coordsLabel��$	��0 0��$��2��$
��Left=�$��140,202,0,0q�$	��Top=�$��16(�
�������$��1���� $��2��$
��Left=�$	��100��$��
10,185,0,0q�$	��Top=�$	��230��$��1���� -   $ label6��$
��Zoom��$��2��$
��Left=�$��
30,236,0,0q�$	��Top=�$��16(�
�������$��1���� -   $ 	zoomLabel��$	��100��$��2��$
��Left=�$��140,236,0,0q�$	��Top=�$��16(�
�������$��1���� -   $ 
zoomSlider��$��2��$
��Left=�$��
35,276,0,0q�$	��Top=�$	��205�� . 
�������$��1�� �� *CurrentZoom+$ TwoWay=�$ PropertyChanged=�$ window��