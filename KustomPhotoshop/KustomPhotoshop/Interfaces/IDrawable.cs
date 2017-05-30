using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;

namespace KustomPhotoshop
{
	public interface IDrawable
	{
		void Initialize(IRenderer renderer);
		void Modify(IRenderer renderer);
		void Draw(IRenderer renderer);
		void Release(IRenderer renderer);

		bool PointIsInside(Point p);
	}
}
