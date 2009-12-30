//---------------------------------------------------------------
//
// Slugs
// box.inl
//
//---------------------------------------------------------------

template <class T>
Box<T>::Box()
{

	center = Vec2<T>(0, 0);
	extents = Vec2<T>(0, 0);

}

template <class T>
Box<T>::Box(const Vec2<T>& _center, const Vec2<T>& _extents)
{

	center = _center;
	extents = _extents;

}

template <class T>
Vec2<T> Box<T>::GetMin() const
{

	return center - extents;

}

template <class T>
Vec2<T> Box<T>::GetMax() const
{

	return center + extents;

}

template <class T>
bool Box<T>::Contains(const Vec2<T>& point) const
{

	return Contains(point.x, point.y);

}

template <class T>
bool Box<T>::Contains(T x, T y) const
{

	Vec2<T> min = GetMin();
	Vec2<T> max = GetMax();

	if ((x >= min.x) && (x <= max.x) && (y >= min.y) && (y <= max.y))
		return true;

	return false;

}

template <class T>
Box<T> Box<T>::Expand(T x, T y) const
{

	return Box(center, Vec2<T>(extents.x + x, extents.y + y));

}