using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;

namespace KustomPhotoshop
{
	public class Ellipse : Figure, IDrawable
	{
		public float a;
		public float b;
		public float angle;


		public Ellipse(Point p1, Point p2, Point p3, Color color_, Color borderColor_, float borderWidth_)
		{
			points = new List<Point>() { p1, p2, p3 };
			color = color_;
			borderColor = borderColor_;
			borderWidth = borderWidth_;

			id = -1;
		}


		public override Point Center
		{
			get
			{
				return new Point((points[0].X + points[1].X) / 2.0, (points[0].Y + points[1].Y) / 2.0);
			}
		}


		public void Initialize(IRenderer renderer)
		{
			double x1x0 = points[1].X - points[0].X;
			double y1y0 = points[1].Y - points[0].Y;

			float a = (float)(Math.Sqrt(x1x0 * x1x0 + y1y0 * y1y0) / 2.0);
			float b = (float)(Math.Abs(y1y0 * points[2].X - x1x0 * points[2].Y + points[1].X * points[0].Y - points[1].Y * points[0].X) / Math.Sqrt(y1y0 * y1y0 + x1x0 * x1x0));
			float angle = (float)Math.Atan(y1y0 / x1x0);

			id = renderer.AddEllipse(Center, a, b, angle, color, borderColor, borderWidth);
		}


		public void Modify(IRenderer renderer)
		{
			if(ID != -1)
			{
				double x1x0 = points[1].X - points[0].X;
				double y1y0 = points[1].Y - points[0].Y;

				float a = (float)(Math.Sqrt(x1x0 * x1x0 + y1y0 * y1y0) / 2.0);
				float b = (float)(Math.Abs(y1y0 * points[2].X - x1x0 * points[2].Y + points[1].X * points[0].Y - points[1].Y * points[0].X) / Math.Sqrt(y1y0 * y1y0 + x1x0 * x1x0));
				float angle = (float)Math.Atan(y1y0 / x1x0);

				renderer.ModifyEllipse(ID, Center, a, b, angle, color, borderColor, borderWidth);
			}
		}


		public void Draw(IRenderer renderer)
		{
			renderer.RenderEllipse(ID);
		}


		public void Release(IRenderer renderer)
		{
			renderer.RemoveEllipse(ID);
		}


		public override List<Point> BoundingBoxPoints
		{
			get
			{
				double x1x0 = points[1].X - points[0].X;
				double y1y0 = points[1].Y - points[0].Y;

				double a = Math.Sqrt(x1x0 * x1x0 + y1y0 * y1y0) / 2.0;
				double b = Math.Abs(y1y0 * points[2].X - x1x0 * points[2].Y + points[1].X * points[0].Y - points[1].Y * points[0].X) / Math.Sqrt(y1y0 * y1y0 + x1x0 * x1x0);
				double angle = Math.Atan(y1y0 / x1x0);
				double sina = Math.Sin(angle);
				double cosa = Math.Cos(angle);

				double ax = a * a * cosa * cosa + b * b * sina * sina;
				double ay = a * a * sina * sina + b * b * cosa * cosa;

				double denominator = cosa * cosa * sina * sina * (a * a - b * b) * (a * a - b * b) - ax * ay;

				double x = Math.Sqrt((-ax * a * a * b * b) / denominator);
				double y = Math.Sqrt((-ay * a * a * b * b) / denominator);

				Point o = Center;

				return new List<Point>() { new Point(o.X - x, o.Y - y), new Point(o.X + x, o.Y + y) };
			}
		}


		public override bool PointIsInside(Point p)
		{
			double x1x0 = points[1].X - points[0].X;
			double y1y0 = points[1].Y - points[0].Y;

			double a = Math.Sqrt(x1x0 * x1x0 + y1y0 * y1y0) / 2.0;
			double b = Math.Abs(y1y0 * points[2].X - x1x0 * points[2].Y + points[1].X * points[0].Y - points[1].Y * points[0].X) / Math.Sqrt(y1y0 * y1y0 + x1x0 * x1x0);
			double angle = Math.Atan(y1y0 / x1x0);

			Point o = Center;
			Point translatedP = new Point(p.X - o.X, p.Y - o.Y);
			Point rotatedP = new Point(translatedP.X * Math.Cos(-angle) - translatedP.Y * Math.Sin(-angle), translatedP.X * Math.Sin(-angle) + translatedP.Y * Math.Cos(-angle));

			if((rotatedP.X * rotatedP.X) / (a * a) + (rotatedP.Y * rotatedP.Y) / (b * b) < 1.0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}
