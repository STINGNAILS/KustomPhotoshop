using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;

namespace KustomPhotoshop
{
	public class Triangle : Figure, IDrawable
	{
		public Triangle(Point p1, Point p2, Point p3, Color color_, Color borderColor_, float borderWidth_)
		{
			points = new List<Point>();
			points.Add(p1);
			points.Add(p2);
			points.Add(p3);
			color = color_;
			borderColor = borderColor_;
			borderWidth = borderWidth_;

			id = -1;
		}


		public void Initialize(IRenderer renderer)
		{
			id = renderer.AddTriangle(points, color, borderColor, borderWidth);
		}


		public void Modify(IRenderer renderer)
		{
			if(id != -1)
			{
				renderer.ModifyTriangle(ID, points, color, borderColor, borderWidth);
			}
		}


		public void Draw(IRenderer renderer)
		{
			if(id != -1)
			{
				renderer.RenderTriangle(ID);
			}
		}


		public void Release(IRenderer renderer)
		{
			if(id != -1)
			{
				renderer.RemoveTriangle(ID);
			}
		}


		public bool PointIsInside(Point p)
		{
			Point a = points[0];
			Point b = points[1];
			Point c = points[2];

			double s1 = Math.Abs((b.X - a.X) * (p.Y - a.Y) - (p.X - a.X) * (b.Y - a.Y));
			double s2 = Math.Abs((c.X - b.X) * (p.Y - b.Y) - (p.X - b.X) * (c.Y - b.Y));
			double s3 = Math.Abs((a.X - c.X) * (p.Y - c.Y) - (p.X - c.X) * (a.Y - c.Y));
			double s = Math.Abs((b.X - a.X) * (c.Y - a.Y) - (c.X - a.X) * (b.Y - a.Y));

			if(Math.Abs(s1 + s2 + s3 - s) < 0.001)
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
