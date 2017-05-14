using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace KustomPhotoshop
{
	public interface ITransform
	{
		Point Transform(Point p);
	}
}
