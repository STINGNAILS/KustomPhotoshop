using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace KustomPhotoshop
{
	public class Translate : ITransform
	{
		private Point d;


		public Translate(Point delta)
		{
			d = delta;
		}


		public Point Transform(Point p)
		{
			return new Point(p.X + d.X, p.Y + d.Y);
		}
	}
}
