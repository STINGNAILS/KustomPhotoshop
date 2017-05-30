using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;

namespace KustomPhotoshop
{
	public class PolyLine : Figure, IDrawable
	{
		public PolyLine(List<Point> points_, Color color_, float borderWidth_)
		{
			points = points_;
			color = color_;
			borderWidth = borderWidth_;

			id = -1;
		}


		public void Initialize(IRenderer renderer)
		{
			id = renderer.AddPolyLine(points, color, borderWidth);
		}


		public void Modify(IRenderer renderer)
		{
			if(id != -1)
			{
				renderer.ModifyPolyLine(ID, points, color, borderWidth);
			}
		}


		public void Draw(IRenderer renderer)
		{
			if(id != -1)
			{
				renderer.RenderPolyLine(ID);
			}
		}


		public void Release(IRenderer renderer)
		{
			if(id != -1)
			{
				renderer.RemovePolyLine(ID);
			}
		}


		public bool PointIsInside(Point p)
		{
			for(int i = 0; i < points.Count - 1; i++)
			{
				Point a = points[i];
				Point b = points[i + 1];

				if(Math.Abs((p.X - a.X) / (b.X - a.X) - (p.Y - a.Y) / (b.Y - a.Y)) < 0.1)
				{
					return true;
				}
			}

			return false;
		}
	}
}
