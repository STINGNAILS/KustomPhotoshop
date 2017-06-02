using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;

namespace KustomPhotoshop
{
	public class Quad : Figure, IDrawable
	{
		public Quad(Point p1, Point p2, Point p3, Point p4, Color color_, Color borderColor_, float borderWidth_)
		{
			if(IsConvex(p1, p2, p3, p4))
			{
				points = new List<Point>() { p1, p2, p3, p4 };
			}
			else if(IsConvex(p1, p2, p4, p3))
			{
				points = new List<Point>() { p1, p2, p4, p3 };
			}
			else if(IsConvex(p1, p3, p2, p4))
			{
				points = new List<Point>() { p1, p3, p2, p4 };
			}
			else
			{
				if(HalfPlane(p1, p3, p2) * HalfPlane(p1, p3, p4) < 0)
				{
					points = new List<Point>() { p1, p2, p3, p4 };
				}
				else
				{
					points = new List<Point>() { p2, p3, p4, p1 };
				}
			}

			color = color_;
			borderColor = borderColor_;
			borderWidth = borderWidth_;

			id = -1;
		}


		public void Initialize(IRenderer renderer)
		{
			id = renderer.AddQuad(points, color, borderColor, borderWidth);
		}


		public void Modify(IRenderer renderer)
		{
			if(id != -1)
			{
				if(IsConvex(points[0], points[1], points[2], points[3]))
				{
					points = new List<Point>() { points[0], points[1], points[2], points[3] };
				}
				else if(IsConvex(points[0], points[1], points[3], points[2]))
				{
					points = new List<Point>() { points[0], points[1], points[3], points[2] };
				}
				else if(IsConvex(points[0], points[2], points[1], points[3]))
				{
					points = new List<Point>() { points[0], points[2], points[1], points[3] };
				}
				else
				{
					if(HalfPlane(points[0], points[2], points[1]) * HalfPlane(points[0], points[2], points[3]) < 0)
					{
						points = new List<Point>() { points[0], points[1], points[2], points[3] };
					}
					else
					{
						points = new List<Point>() { points[1], points[2], points[3], points[0] };
					}
				}

				renderer.ModifyQuad(ID, points, color, borderColor, borderWidth);
			}
		}


		public void Draw(IRenderer renderer)
		{
			if(id != -1)
			{
				renderer.RenderQuad(ID);
			}
		}


		public void Release(IRenderer renderer)
		{
			if(id != -1)
			{
				renderer.RemoveQuad(ID);
			}
		}


		public override bool PointIsInside(Point p)
		{
			Point a = points[0];
			Point b = points[1];
			Point c = points[2];
			Point d = points[3];

			double s1 = Math.Abs((b.X - a.X) * (p.Y - a.Y) - (p.X - a.X) * (b.Y - a.Y));
			double s2 = Math.Abs((c.X - b.X) * (p.Y - b.Y) - (p.X - b.X) * (c.Y - b.Y));
			double s3 = Math.Abs((d.X - c.X) * (p.Y - c.Y) - (p.X - c.X) * (d.Y - c.Y));
			double s4 = Math.Abs((a.X - d.X) * (p.Y - d.Y) - (p.X - d.X) * (a.Y - d.Y));
			double s = Math.Abs((a.X - b.X) * (a.Y + b.Y) + (b.X - c.X) * (b.Y + c.Y) + (c.X - d.X) * (c.Y + d.Y) + (d.X - a.X) * (d.Y + a.Y));

			if(Math.Abs(s1 + s2 + s3 + s4 - s) < 0.001)
			{
				return true;
			}
			else
			{
				return false;
			}
		}


		private bool IsConvex(Point a, Point b, Point c, Point d)
		{
			if(HalfPlane(a, c, b) * HalfPlane(a, c, d) > 0)
			{
				return false;
			}

			if(HalfPlane(b, d, a) * HalfPlane(b, d, c) > 0)
			{
				return false;
			}

			return true;
		}


		private float HalfPlane(Point a, Point b, Point p)
		{
			return (float)((a.Y - b.Y) * p.X + (b.X - a.X) * p.Y + (a.X * b.Y - b.X * a.Y));
		}


		public bool IsConvex()
		{
			return IsConvex(points[0], points[1], points[2], points[3]);
		}
	}
}