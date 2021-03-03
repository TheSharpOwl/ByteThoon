#pragma once
/*
* This functions might not be used in the project but I kept it since it is one of the things I learned 
* during making this project and since it's a personal project I just kept it here ))
*/

// CPP standard can u please add + operator to std::pair))
// call me crazy I don't care
template<typename T, typename U>
std::pair<T, U> operator+(const std::pair<T, U>& l, const std::pair<T, U>& r)
{
	return { l.first + r.first, l.second + r.second };
}

template<typename T, typename U>
std::pair<T, U>& operator+=(std::pair<T, U>& l, const std::pair<T, U>& r)
{
	l.first += r.first;
	l.second += r.second;
	return l;
}