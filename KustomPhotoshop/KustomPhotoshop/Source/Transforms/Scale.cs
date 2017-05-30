using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace KustomPhotoshop
{
	public class Scale : ITransform
	{
		private float xScale;
		private float yScale;
		private Point center;


		public Scale(float xScale_, float yScale_, Point center_)
		{
			xScale = xScale_;
			yScale = yScale_;
			center = center_;
		}


		public Point Transform(Point p)
		{
			return new Point(center.X + (p.X - center.X) * xScale, center.Y + (p.Y - center.Y) * yScale);
		}
	}
}
