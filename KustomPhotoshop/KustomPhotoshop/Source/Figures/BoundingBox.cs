using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace KustomPhotoshop
{
	public enum Boundary
	{
		NONE,
		TOP,
		LEFT,
		RIGHT,
		BOTTOM,
		TOPLEFT,
		TOPRIGHT,
		BOTTOMLEFT,
		BOTTOMRIGHT
	}


	public class BoundingBox : Figure, IDrawable
	{
		public BoundingBox(Point pMin, Point pMax)
		{
			points = new List<Point>();
			points.Add(pMin);
			points.Add(pMax);
		}


		public void Draw(IRenderer renderer)
		{
			renderer.RenderBoundingBox();
		}


		public void Initialize(IRenderer renderer)
		{
			
		}


		public void Modify(IRenderer renderer)
		{
			renderer.ModifyBoundingBox(points);
		}


		public bool PointIsInside(Point p)
		{
			return false;
		}


		public void Release(IRenderer renderer)
		{
			
		}


		public Boundary PointIsOnBoundary(Point p)
		{
			if(Math.Abs(p.Y - points[1].Y) < 3.0f)
			{
				if(Math.Abs(p.X - points[0].X) < 3.0f)
				{
					return Boundary.TOPLEFT;
				}
				else if(Math.Abs(p.X - points[1].X) < 3.0f)
				{
					return Boundary.TOPRIGHT;
				}
				else if(p.X > points[0].X && p.X < points[1].X)
				{
					return Boundary.TOP;
				}
			}
			else if(Math.Abs(p.X - points[0].X) < 3.0f)
			{
				if(Math.Abs(p.Y - points[0].Y) < 3.0f)
				{
					return Boundary.BOTTOMLEFT;
				}
				else if(p.Y > points[0].Y && p.Y < points[1].Y)
				{
					return Boundary.LEFT;
				}
			}
			else if(Math.Abs(p.X - points[1].X) < 3.0f)
			{
				if(Math.Abs(p.Y - points[0].Y) < 3.0f)
				{
					return Boundary.BOTTOMRIGHT;
				}
				else if(p.Y > points[0].Y && p.Y < points[1].Y)
				{
					return Boundary.RIGHT;
				}
			}
			else if(Math.Abs(p.Y - points[0].Y) < 3.0f)
			{
				if(p.X > points[0].X && p.X < points[1].X)
				{
					return Boundary.BOTTOM;
				}
			}

			return Boundary.NONE;
		}
	}
}
