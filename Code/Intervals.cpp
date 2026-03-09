
typedef pair<int,int> Point;

vector<Point> Intersection(vector<Point>  &a, vector<Point>  &b)
{
	vector<Point> out;
	int i = 0;
	int j = 0;
	while (i < a.size() &&
			j < b.size())
	{
		int s1 = a[i].first;
		int e1 = a[i].second;
		int s2 = b[j].first;
		int e2 = b[j].second;
		if (isContain(a[i], b[j]) {
			out.push_back(Point(max(s1,s2), min(e1, e2)));
		}
		if (e1 < e2) {
			++i;
		} else {
			++j;
		}
	}
	return out;
}

bool isContain(Point a, Point b) {
	if a.first >= b.first && a.first < b.second return true;
	if b.first >= a.first && b.first < a.second return true;
	return false;
}

vector<Point> Union(vector<Point> const &a, vector<Point> const &b)
{
	vector<Point> out;
	int i = 0;
	int j = 0;

	while (i < a.size() && j < b.size()) {
		if (a[i].first < b[j].first) {
			s = a[i].first;
			e = a[i].second;
			++i;
		} else {
			s = b[j].first;
			e = b[j].second;
			++j;
		}

		if (isContain(out.back(), P(s,e)) && 
			out.back().second = max(out.back().second, e)));
		} else {
			out.push_back(Point(s, e);
		}
	}
	return out;
}