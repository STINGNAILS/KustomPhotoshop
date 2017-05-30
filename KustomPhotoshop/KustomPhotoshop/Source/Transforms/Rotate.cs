using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace KustomPhotoshop
{
	public class Rotate : ITransform
	{
		private float angle;
		private Point center;


		public Rotate(float angle_, Point center_)
		{
			angle = angle_;
			center = center_;
		}


		public Point Transform(Point p)
		{
			return new Point(center.X + (p.X - center.X) * Math.Cos(angle) - (p.Y - center.Y) * Math.Sin(angle), center.Y + (p.X - center.X) * Math.Sin(angle) + (p.Y - center.Y) * Math.Cos(angle));
		}
	}
}
