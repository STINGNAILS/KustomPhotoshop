using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;

namespace KustomPhotoshop
{
	public abstract class Figure
	{
		protected int id;

		public List<Point> points;
		public Color color;
		public Color borderColor;
		public float borderWidth;

		public int ID
		{
			get
			{
				return id;
			}
		}

		public Point Center
		{
			get
			{
				Point center = new Point(0.0, 0.0);

				foreach(Point p in points)
				{
					center.X += p.X;
					center.Y += p.Y;
				}

				center.X /= points.Count;
				center.Y /= points.Count;

				return center;
			}
		}

		public List<Point> BoundingBoxPoints
		{
			get
			{
				double xMin = points[0].X;
				double yMin = points[0].Y;
				double xMax = points[0].X;
				double yMax = points[0].Y;

				foreach(Point point in points)
				{
					if(point.X < xMin)
					{
						xMin = point.X;
					}
					else if(point.X > xMax)
					{
						xMax = point.X;
					}

					if(point.Y < yMin)
					{
						yMin = point.Y;
					}
					else if(point.Y > yMax)
					{
						yMax = point.Y;
					}
				}

				List<Point> boundingBoxPoints = new List<Point>();
				boundingBoxPoints.Add(new Point(xMin, yMin));
				boundingBoxPoints.Add(new Point(xMax, yMax));

				return boundingBoxPoints;
			}
		}


		public void ApplyTransform(ITransform transform)
		{
			for(int i = 0; i < points.Count; i++)
			{
				points[i] = transform.Transform(points[i]);
			}
		}
	}
}
