using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Forms;
using System.ComponentModel;
using Xceed.Wpf.Toolkit;

namespace KustomPhotoshop
{
	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>

	public enum DrawMode
	{
		NONE,
		POLYLINE,
		TRIANGLE,
		QUAD,
		ELLIPSE
	}


	public partial class MainWindow : Window, INotifyPropertyChanged
	{
		private DirectXRenderer directXRenderer;

        private List<IDrawable> figures;
		private IDrawable currentFigure;
		private IDrawable formingFigure;
		private BoundingBox boundingBox;

		private List<Point> formingPoints;
		private DrawMode drawMode;

		private Color currentColor;
		private Color currentBorderColor;
		private float currentBorderWidth;

		private Point cameraCoords;
		private float cameraZoom;

		private bool leftMouseButtonIsPressed;
		private bool middleMouseButtonIsPressed;
		private bool rightMouseButtonIsPressed;
		private Point leftMouseClickCoords;
		private Point middleMouseClickCoords;
		private Point rightMouseClickCoords;

		private bool isTranslating;

		private Boundary upscalingBoundary;
		private Point lastScalingPoint;

		public event PropertyChangedEventHandler PropertyChanged;


		public Color CurrentColor
		{
			get
			{
				return currentColor;
			}
			set
			{
				currentColor = value;

				if(currentFigure != null)
				{
					(currentFigure as Figure).color = currentColor;
					ModifyFigure(currentFigure, directXRenderer);
					directXControl.Invalidate();
				}

				PropertyChanged(this, new PropertyChangedEventArgs("CurrentColor"));
			}
		}


		public Color CurrentBorderColor
		{
			get
			{
				return currentBorderColor;
			}
			set
			{
				currentBorderColor = value;

				if(currentFigure != null)
				{
					(currentFigure as Figure).borderColor = currentBorderColor;
					ModifyFigure(currentFigure, directXRenderer);
					directXControl.Invalidate();
				}

				PropertyChanged(this, new PropertyChangedEventArgs("CurrentBorderColor"));
			}
		}


		public float CurrentBorderWidth
		{
			get
			{
				return currentBorderWidth;
			}
			set
			{
				currentBorderWidth = value;
				widthLabel.Content = currentBorderWidth.ToString("N2");

				if(currentFigure != null)
				{
					(currentFigure as Figure).borderWidth = currentBorderWidth;
					ModifyFigure(currentFigure, directXRenderer);
					directXControl.Invalidate();
				}

				PropertyChanged(this, new PropertyChangedEventArgs("CurrentBorderWidth"));
			}
		}


		public Point CurrentCoords
		{
			get
			{
				return cameraCoords;
			}
			set
			{
				cameraCoords = value;
				coordsLabel.Content = cameraCoords.X.ToString("N0") + ", " + cameraCoords.Y.ToString("N0");

				directXControl.SetCameraPosition(cameraCoords);
				directXControl.Invalidate();

				PropertyChanged(this, new PropertyChangedEventArgs("CurrentCoords"));
			}
		}


		public float CurrentZoom
		{
			get
			{
				return cameraZoom;
			}
			set
			{
				cameraZoom = value;
				cameraZoom = cameraZoom >= 0.1f ? cameraZoom : 0.1f;
				cameraZoom = cameraZoom <= 10.0f ? cameraZoom : 10.0f;

				zoomLabel.Content = (cameraZoom * 100).ToString("N0") + "%";

				directXControl.SetCameraZoom(cameraZoom);
				directXControl.SetBoundingBoxZoom(cameraZoom);
				directXControl.Invalidate();

				PropertyChanged(this, new PropertyChangedEventArgs("CurrentZoom"));
			}
		}


		public MainWindow()
		{
			InitializeComponent();

			directXRenderer = new DirectXRenderer();

			directXControl.Init();

			figures = new List<IDrawable>();
			currentFigure = null;
			formingFigure = null;
			boundingBox = null;

			formingPoints = null;
			drawMode = DrawMode.NONE;

			CurrentColor = Color.FromRgb(255, 255, 255);
			CurrentBorderColor = Color.FromRgb(0, 0, 0);
			CurrentBorderWidth = 1.0f;

			CurrentCoords = new Point(0.0, 0.0);
			CurrentZoom = 1.0f;

			leftMouseButtonIsPressed = false;
			middleMouseButtonIsPressed = false;
			rightMouseButtonIsPressed = false;

			isTranslating = false;

			upscalingBoundary = Boundary.NONE;
		}


		public void InitializeFigure(IDrawable figure, IRenderer renderer)
		{
			if(figure != null)
			{
				figure.Initialize(renderer);
			}
		}


		public void ModifyFigure(IDrawable figure, IRenderer renderer)
		{
			if(figure != null)
			{
				figure.Modify(renderer);
			}
		}


		public void DrawFigure(IDrawable figure, IRenderer renderer)
		{
			if(figure != null)
			{
				figure.Draw(renderer);
			}
		}


		public void ReleaseFigure(IDrawable figure, IRenderer renderer)
		{
			if(figure != null)
			{
				figure.Release(renderer);
			}
		}


		private void OnMouseDown(MouseButton button, int xMouse, int yMouse)
		{
			float x = (float)cameraCoords.X + (xMouse - directXControl.Size.Width / 2.0f) / CurrentZoom;
			float y = (float)cameraCoords.Y + (directXControl.Size.Height / 2.0f - yMouse) / CurrentZoom;

			switch(button)
			{
				case MouseButton.Left:
				{
					leftMouseClickCoords = new Point(xMouse, yMouse);
					leftMouseButtonIsPressed = true;

					switch(drawMode)
					{
						case DrawMode.NONE:
						{
							if(currentFigure != null)
							{
								upscalingBoundary = boundingBox.PointIsOnBoundary(new Point(x, y), CurrentZoom);
								lastScalingPoint = new Point(x, y);
							}

							if(upscalingBoundary == Boundary.NONE)
							{
								int i = figures.Count - 1;
								for(; i >= 0; i--)
								{
									if(figures[i].PointIsInside(new Point(x, y)))
									{
										currentFigure = figures[i];

										CurrentColor = (currentFigure as Figure).color;
										CurrentBorderColor = (currentFigure as Figure).borderColor;
										CurrentBorderWidth = (currentFigure as Figure).borderWidth;

										List<Point> boundingBoxPoints = (currentFigure as Figure).BoundingBoxPoints;
										boundingBox = new BoundingBox(boundingBoxPoints[0], boundingBoxPoints[1]);
										ModifyFigure(boundingBox, directXRenderer);

										isTranslating = true;
										directXControl.Invalidate();

										break;
									}
								}

								if(i < 0)
								{
									currentFigure = null;
									boundingBox = null;
									directXControl.Invalidate();
								}
							}
							break;
						}
						case DrawMode.POLYLINE:
						{
							formingPoints.Add(new Point(x, y));
							if(formingPoints.Count == 1)
							{
								List<Point> currentPoints = new List<Point>(formingPoints);
								currentPoints.Add(new Point(x, y));
								formingFigure = new PolyLine(currentPoints, CurrentBorderColor, CurrentBorderWidth);
								InitializeFigure(formingFigure, directXRenderer);
							}
							else if(formingPoints.Count >= 6)
							{
								ReleaseFigure(formingFigure, directXRenderer);
								formingFigure = null;
								PolyLine polyLine = new PolyLine(formingPoints, CurrentColor, CurrentBorderWidth);
								InitializeFigure(polyLine, directXRenderer);
								figures.Add(polyLine);
								drawMode = DrawMode.NONE;

								polyLineButton.IsChecked = false;
								directXControl.Invalidate();
							}
							break;
						}
						case DrawMode.TRIANGLE:
						{
							formingPoints.Add(new Point(x, y));
							if(formingPoints.Count == 1)
							{
								List<Point> currentPoints = new List<Point>(formingPoints);
								currentPoints.Add(new Point(x, y));
								formingFigure = new PolyLine(currentPoints, CurrentColor, CurrentBorderWidth);
								InitializeFigure(formingFigure, directXRenderer);
							}
							else if(formingPoints.Count == 2)
							{
								ReleaseFigure(formingFigure, directXRenderer);
								formingFigure = new Triangle(formingPoints[0], formingPoints[1], formingPoints[1], CurrentColor, CurrentBorderColor, CurrentBorderWidth);
								InitializeFigure(formingFigure, directXRenderer);
							}
							else if(formingPoints.Count >= 3)
							{
								ReleaseFigure(formingFigure, directXRenderer);
								formingFigure = null;
								Triangle triangle = new Triangle(formingPoints[0], formingPoints[1], formingPoints[2], CurrentColor, CurrentBorderColor, CurrentBorderWidth);
								InitializeFigure(triangle, directXRenderer);
								figures.Add(triangle);
								drawMode = DrawMode.NONE;

								triangleButton.IsChecked = false;
								directXControl.Invalidate();
							}
							break;
						}
						case DrawMode.QUAD:
						{
							formingPoints.Add(new Point(x, y));
							if(formingPoints.Count == 1)
							{
								List<Point> currentPoints = new List<Point>(formingPoints);
								currentPoints.Add(new Point(x, y));
								formingFigure = new PolyLine(currentPoints, CurrentColor, CurrentBorderWidth);
								InitializeFigure(formingFigure, directXRenderer);
							}
							else if(formingPoints.Count == 2)
							{
								ReleaseFigure(formingFigure, directXRenderer);
								formingFigure = new Triangle(formingPoints[0], formingPoints[1], formingPoints[1], CurrentColor, CurrentBorderColor, CurrentBorderWidth);
								InitializeFigure(formingFigure, directXRenderer);
							}
							else if(formingPoints.Count == 3)
							{
								ReleaseFigure(formingFigure, directXRenderer);
								formingFigure = new Quad(formingPoints[0], formingPoints[1], formingPoints[2], formingPoints[2], CurrentColor, CurrentBorderColor, CurrentBorderWidth);
								InitializeFigure(formingFigure, directXRenderer);
							}
							else if(formingPoints.Count >= 4)
							{
								if((formingFigure as Quad).IsConvex())
								{
									Quad quad = new Quad(formingPoints[0], formingPoints[1], formingPoints[2], formingPoints[3], CurrentColor, CurrentBorderColor, CurrentBorderWidth);
									InitializeFigure(quad, directXRenderer);
									figures.Add(quad);
								}

								ReleaseFigure(formingFigure, directXRenderer);
								formingFigure = null;
								drawMode = DrawMode.NONE;

								quadButton.IsChecked = false;
								directXControl.Invalidate();
							}
							break;
						}
						case DrawMode.ELLIPSE:
						{
							formingPoints.Add(new Point(x, y));
							if(formingPoints.Count == 1)
							{
								List<Point> currentPoints = new List<Point>(formingPoints);
								currentPoints.Add(new Point(x, y));
								formingFigure = new PolyLine(currentPoints, CurrentColor, CurrentBorderWidth);
								InitializeFigure(formingFigure, directXRenderer);
							}
							else if(formingPoints.Count == 2)
							{
								ReleaseFigure(formingFigure, directXRenderer);
								formingFigure = new Ellipse(formingPoints[0], formingPoints[1], formingPoints[1], CurrentColor, CurrentBorderColor, CurrentBorderWidth);
								InitializeFigure(formingFigure, directXRenderer);
							}
							else if(formingPoints.Count >= 3)
							{
								ReleaseFigure(formingFigure, directXRenderer);
								formingFigure = null;
								Ellipse ellipse = new Ellipse(formingPoints[0], formingPoints[1], formingPoints[2], CurrentColor, CurrentBorderColor, CurrentBorderWidth);
								InitializeFigure(ellipse, directXRenderer);
								figures.Add(ellipse);
								drawMode = DrawMode.NONE;

								ellipseButton.IsChecked = false;
								directXControl.Invalidate();
							}
							break;
						}
					}
					break;
				}
				case MouseButton.Middle:
				{
					middleMouseClickCoords = new Point(xMouse, yMouse);
					middleMouseButtonIsPressed = true;

					break;
				}
				case MouseButton.Right:
				{
					rightMouseClickCoords = new Point(xMouse, yMouse);
					rightMouseButtonIsPressed = true;

					break;
				}
			}
		}


		private void OnMouseMove(int xMouse, int yMouse)
		{
			float x = (float)CurrentCoords.X + (xMouse - directXControl.Size.Width / 2.0f) / CurrentZoom;
			float y = (float)CurrentCoords.Y + (directXControl.Size.Height / 2.0f - yMouse) / CurrentZoom;

			switch(drawMode)
			{
				case DrawMode.NONE:
				{
					if(currentFigure != null)
					{
						switch(boundingBox.PointIsOnBoundary(new Point(x, y), CurrentZoom))
						{
							case Boundary.NONE:
							{
								Mouse.OverrideCursor = null;
								break;
							}
							case Boundary.TOP:
							{
								Mouse.OverrideCursor = System.Windows.Input.Cursors.SizeNS;
								break;
							}
							case Boundary.LEFT:
							{
								Mouse.OverrideCursor = System.Windows.Input.Cursors.SizeWE;
								break;
							}
							case Boundary.RIGHT:
							{
								Mouse.OverrideCursor = System.Windows.Input.Cursors.SizeWE;
								break;
							}
							case Boundary.BOTTOM:
							{
								Mouse.OverrideCursor = System.Windows.Input.Cursors.SizeNS;
								break;
							}
							case Boundary.TOPLEFT:
							{
								Mouse.OverrideCursor = System.Windows.Input.Cursors.SizeNWSE;
								break;
							}
							case Boundary.TOPRIGHT:
							{
								Mouse.OverrideCursor = System.Windows.Input.Cursors.SizeNESW;
								break;
							}
							case Boundary.BOTTOMLEFT:
							{
								Mouse.OverrideCursor = System.Windows.Input.Cursors.SizeNESW;
								break;
							}
							case Boundary.BOTTOMRIGHT:
							{
								Mouse.OverrideCursor = System.Windows.Input.Cursors.SizeNWSE;
								break;
							}
						}
					}
					if(leftMouseButtonIsPressed && !middleMouseButtonIsPressed && !rightMouseButtonIsPressed && currentFigure != null)
					{
						if(upscalingBoundary != Boundary.NONE)
						{
							Point center = (currentFigure as Figure).Center;

							float xScale = 1.0f, yScale = 1.0f;
							if(upscalingBoundary == Boundary.RIGHT || upscalingBoundary == Boundary.TOPRIGHT || upscalingBoundary == Boundary.BOTTOMRIGHT)
							{
								xScale = (float)(x - lastScalingPoint.X + boundingBox.points[1].X - boundingBox.points[0].X) / (float)(boundingBox.points[1].X - boundingBox.points[0].X);
								center.X = boundingBox.points[0].X;
							}
							else if(upscalingBoundary == Boundary.LEFT || upscalingBoundary == Boundary.TOPLEFT || upscalingBoundary == Boundary.BOTTOMLEFT)
							{
								xScale = (float)(lastScalingPoint.X - x + boundingBox.points[1].X - boundingBox.points[0].X) / (float)(boundingBox.points[1].X - boundingBox.points[0].X);
								center.X = boundingBox.points[1].X;
							}
							if(upscalingBoundary == Boundary.TOP || upscalingBoundary == Boundary.TOPLEFT || upscalingBoundary == Boundary.TOPRIGHT)
							{
								yScale = (float)(y - lastScalingPoint.Y + boundingBox.points[1].Y - boundingBox.points[0].Y) / (float)(boundingBox.points[1].Y - boundingBox.points[0].Y);
								center.Y = boundingBox.points[0].Y;
							}
							else if(upscalingBoundary == Boundary.BOTTOM || upscalingBoundary == Boundary.BOTTOMLEFT || upscalingBoundary == Boundary.BOTTOMRIGHT)
							{
								yScale = (float)(lastScalingPoint.Y - y + boundingBox.points[1].Y - boundingBox.points[0].Y) / (float)(boundingBox.points[1].Y - boundingBox.points[0].Y);
								center.Y = boundingBox.points[1].Y;
							}

							Scale scale = new Scale(xScale, yScale, center);
							(currentFigure as Figure).ApplyTransform(scale);
							ModifyFigure(currentFigure, directXRenderer);

							List<Point> boundingBoxPoints = (currentFigure as Figure).BoundingBoxPoints;
							boundingBox = new BoundingBox(boundingBoxPoints[0], boundingBoxPoints[1]);
							ModifyFigure(boundingBox, directXRenderer);

							lastScalingPoint = new Point(x, y);

							directXControl.Invalidate();
						}
						else if(isTranslating)
						{
							Translate translate = new Translate(new Point((xMouse - leftMouseClickCoords.X) / CurrentZoom, (leftMouseClickCoords.Y - yMouse) / CurrentZoom));
							(currentFigure as Figure).ApplyTransform(translate);
							ModifyFigure(currentFigure, directXRenderer);

							List<Point> boundingBoxPoints = (currentFigure as Figure).BoundingBoxPoints;
							boundingBox = new BoundingBox(boundingBoxPoints[0], boundingBoxPoints[1]);
							ModifyFigure(boundingBox, directXRenderer);

							leftMouseClickCoords = new Point(xMouse, yMouse);

							directXControl.Invalidate();
						}
					}
					else if(!leftMouseButtonIsPressed && middleMouseButtonIsPressed && !rightMouseButtonIsPressed && currentFigure != null)
					{
						Rotate rotate = new Rotate((float)(middleMouseClickCoords.X - xMouse) * 0.005f, (currentFigure as Figure).Center);
						(currentFigure as Figure).ApplyTransform(rotate);
						ModifyFigure(currentFigure, directXRenderer);

						List<Point> boundingBoxPoints = (currentFigure as Figure).BoundingBoxPoints;
						boundingBox = new BoundingBox(boundingBoxPoints[0], boundingBoxPoints[1]);
						ModifyFigure(boundingBox, directXRenderer);

						middleMouseClickCoords = new Point(xMouse, yMouse);

						directXControl.Invalidate();
					}
					break;
				}
				case DrawMode.POLYLINE:
				{
					if(formingPoints.Count >= 1)
					{
						List<Point> currentPoints = new List<Point>(formingPoints);
						currentPoints.Add(new Point(x, y));
						(formingFigure as PolyLine).points = currentPoints;
						(formingFigure as PolyLine).color = CurrentColor;
						(formingFigure as PolyLine).borderColor = CurrentBorderColor;
						(formingFigure as PolyLine).borderWidth = CurrentBorderWidth;
						ModifyFigure(formingFigure, directXRenderer);

						directXControl.Invalidate();
					}
					break;
				}
				case DrawMode.TRIANGLE:
				{
					if(formingPoints.Count == 1)
					{
						List<Point> currentPoints = new List<Point>(formingPoints);
						currentPoints.Add(new Point(x, y));
						(formingFigure as Figure).points = currentPoints;
						(formingFigure as Figure).color = CurrentBorderColor;
						(formingFigure as Figure).borderColor = CurrentBorderColor;
						(formingFigure as Figure).borderWidth = CurrentBorderWidth;
						ModifyFigure(formingFigure, directXRenderer);

						directXControl.Invalidate();
					}
					else if(formingPoints.Count == 2)
					{
						List<Point> currentPoints = new List<Point>(formingPoints);
						currentPoints.Add(new Point(x, y));
						(formingFigure as Figure).points = currentPoints;
						(formingFigure as Figure).color = CurrentColor;
						(formingFigure as Figure).borderColor = CurrentBorderColor;
						(formingFigure as Figure).borderWidth = CurrentBorderWidth;
						ModifyFigure(formingFigure, directXRenderer);

						directXControl.Invalidate();
					}
					break;
				}
				case DrawMode.QUAD:
				{
					if(formingPoints.Count == 1)
					{
						List<Point> currentPoints = new List<Point>(formingPoints);
						currentPoints.Add(new Point(x, y));
						(formingFigure as Figure).points = currentPoints;
						(formingFigure as Figure).color = CurrentBorderColor;
						(formingFigure as Figure).borderColor = CurrentBorderColor;
						(formingFigure as Figure).borderWidth = CurrentBorderWidth;
						ModifyFigure(formingFigure, directXRenderer);

						directXControl.Invalidate();
					}
					else if(formingPoints.Count == 2 || formingPoints.Count == 3)
					{
						List<Point> currentPoints = new List<Point>(formingPoints);
						currentPoints.Add(new Point(x, y));
						(formingFigure as Figure).points = currentPoints;
						(formingFigure as Figure).color = CurrentColor;
						(formingFigure as Figure).borderColor = CurrentBorderColor;
						(formingFigure as Figure).borderWidth = CurrentBorderWidth;
						ModifyFigure(formingFigure, directXRenderer);

						directXControl.Invalidate();
					}
					break;
				}
				case DrawMode.ELLIPSE:
				{
					if(formingPoints.Count == 1)
					{
						List<Point> currentPoints = new List<Point>(formingPoints);
						currentPoints.Add(new Point(x, y));
						(formingFigure as Figure).points = currentPoints;
						(formingFigure as Figure).color = CurrentBorderColor;
						(formingFigure as Figure).borderColor = CurrentBorderColor;
						(formingFigure as Figure).borderWidth = CurrentBorderWidth;
						ModifyFigure(formingFigure, directXRenderer);

						directXControl.Invalidate();
					}
					else if(formingPoints.Count == 2)
					{
						List<Point> currentPoints = new List<Point>(formingPoints);
						currentPoints.Add(new Point(x, y));
						(formingFigure as Figure).points = currentPoints;
						(formingFigure as Figure).color = CurrentColor;
						(formingFigure as Figure).borderColor = CurrentBorderColor;
						(formingFigure as Figure).borderWidth = CurrentBorderWidth;
						ModifyFigure(formingFigure, directXRenderer);

						directXControl.Invalidate();
					}
					break;
				}
			}

			if(rightMouseButtonIsPressed)
			{
				CurrentCoords = new Point(CurrentCoords.X + (rightMouseClickCoords.X - xMouse) / CurrentZoom, CurrentCoords.Y + (yMouse - rightMouseClickCoords.Y) / CurrentZoom);
				rightMouseClickCoords = new Point(xMouse, yMouse);
			}
		}


		private void OnMouseWheel(int delta)
		{
			CurrentZoom *= (float)Math.Pow(1.15, delta / 120.0);
		}


		private void OnMouseUp(MouseButton button)
		{
			switch(button)
			{
				case MouseButton.Left:
				{
					leftMouseButtonIsPressed = false;
					break;
				}
				case MouseButton.Middle:
				{
					middleMouseButtonIsPressed = false;
					break;
				}
				case MouseButton.Right:
				{
					rightMouseButtonIsPressed = false;
					break;
				}
			}

			isTranslating = false;
			upscalingBoundary = Boundary.NONE;
		}


        private void DirectXRenderer_OnMouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
        {
			MouseButton button;
			switch(e.Button)
			{
				case MouseButtons.Left:
				{
					button = MouseButton.Left;
					break;
				}
				case MouseButtons.Middle:
				{
					button = MouseButton.Middle;
					break;
				}
				case MouseButtons.Right:
				{
					button = MouseButton.Right;
					break;
				}
				default:
				{
					button = MouseButton.Right;
					break;
				}
			}

			OnMouseDown(button, e.X, e.Y);
        }


        private void DirectXRenderer_OnMouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
        {
			OnMouseMove(e.X, e.Y);
        }


        private void DirectXRenderer_OnMouseWheel(object sender, System.Windows.Forms.MouseEventArgs e)
        {
			OnMouseWheel(e.Delta);
        }


        private void DirectXRenderer_OnMouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
        {
			MouseButton button;
			switch(e.Button)
			{
				case MouseButtons.Left:
				{
					button = MouseButton.Left;
					break;
				}
				case MouseButtons.Middle:
				{
					button = MouseButton.Middle;
					break;
				}
				case MouseButtons.Right:
				{
					button = MouseButton.Right;
					break;
				}
				default:
				{
					button = MouseButton.Right;
					break;
				}
			}

			OnMouseUp(button);
		}


		private void DirectXControl_SizeChanged(object sender, EventArgs e)
		{
			directXControl.DirectXControl_SizeChanged(sender, e);
			directXControl.Invalidate();
		}


		private void DirectXControl_Paint(object sender, System.Windows.Forms.PaintEventArgs e)
		{
			directXControl.BeginRender();

			if(figures != null)
			{
				foreach(var figure in figures)
				{
					DrawFigure(figure, directXRenderer);
				}
			}

			DrawFigure(formingFigure, directXRenderer);

			DrawFigure(boundingBox, directXRenderer);

			directXControl.FinishRender();
		}


		private void polyLineButton_Checked(object sender, RoutedEventArgs e)
		{
			triangleButton.IsChecked = false;
			quadButton.IsChecked = false;
			ellipseButton.IsChecked = false;

			ReleaseFigure(formingFigure, directXRenderer);
			boundingBox = null;
			currentFigure = null;
			formingFigure = null;
			formingPoints = new List<Point>();
			drawMode = DrawMode.POLYLINE;

			directXControl.Invalidate();
		}


		private void triangleButton_Checked(object sender, RoutedEventArgs e)
		{
			polyLineButton.IsChecked = false;
			quadButton.IsChecked = false;
			ellipseButton.IsChecked = false;

			ReleaseFigure(formingFigure, directXRenderer);
			boundingBox = null;
			currentFigure = null;
			formingFigure = null;
			formingPoints = new List<Point>();
			drawMode = DrawMode.TRIANGLE;

			directXControl.Invalidate();
		}


		private void quadButton_Checked(object sender, RoutedEventArgs e)
		{
			polyLineButton.IsChecked = false;
			triangleButton.IsChecked = false;
			ellipseButton.IsChecked = false;

			ReleaseFigure(formingFigure, directXRenderer);
			boundingBox = null;
			currentFigure = null;
			formingFigure = null;
			formingPoints = new List<Point>();
			drawMode = DrawMode.QUAD;

			directXControl.Invalidate();
		}


		private void ellipseButton_Checked(object sender, RoutedEventArgs e)
		{
			polyLineButton.IsChecked = false;
			triangleButton.IsChecked = false;
			quadButton.IsChecked = false;

			ReleaseFigure(formingFigure, directXRenderer);
			boundingBox = null;
			currentFigure = null;
			formingFigure = null;
			formingPoints = new List<Point>();
			drawMode = DrawMode.ELLIPSE;

			directXControl.Invalidate();
		}


		private void OnExit(object sender, EventArgs e)
		{
			directXControl.Release();
		}


		private void OnKeyDown(object sender, System.Windows.Input.KeyEventArgs e)
		{
			switch(e.Key)
			{
				case Key.Escape:
				{
					if(drawMode == DrawMode.POLYLINE && formingPoints.Count >= 2)
					{
						ReleaseFigure(formingFigure, directXRenderer);
						formingFigure = null;
						PolyLine polyLine = new PolyLine(formingPoints, CurrentColor, CurrentBorderWidth);
						InitializeFigure(polyLine, directXRenderer);
						figures.Add(polyLine);
						drawMode = DrawMode.NONE;
						polyLineButton.IsChecked = false;

						directXControl.Invalidate();
					}
					break;
				}
			}
		}


		private void foregroundButton_Click(object sender, RoutedEventArgs e)
		{
			if(currentFigure != null)
			{
				figures.Remove(currentFigure);
				figures.Add(currentFigure);
				directXControl.Invalidate();
			}
		}


		private void deleteButton_Click(object sender, RoutedEventArgs e)
		{
			if(currentFigure != null)
			{
				ReleaseFigure(currentFigure, directXRenderer);
				figures.Remove(currentFigure);
				currentFigure = null;
				boundingBox = null;
				directXControl.Invalidate();
			}
		}


		private void newMenu_Click(object sender, RoutedEventArgs e)
		{
			CurrentColor = Colors.White;
			CurrentBorderColor = Colors.Black;
			CurrentBorderWidth = 1.0f;

			CurrentCoords = new Point(0.0f, 0.0f);
			CurrentZoom = 1.0f;


			ReleaseFigure(formingFigure, directXRenderer);

			foreach(IDrawable figure in figures)
			{
				ReleaseFigure(figure, directXRenderer);
			}

			figures.Clear();
			currentFigure = null;
			formingFigure = null;
			boundingBox = null;
		}


		private void openMenu_Click(object sender, RoutedEventArgs e)
		{

		}


		private void saveMenu_Click(object sender, RoutedEventArgs e)
		{

		}


		private void exitMenu_Click(object sender, RoutedEventArgs e)
		{
			Close();
		}
	}
}
