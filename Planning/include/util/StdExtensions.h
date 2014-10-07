#ifndef PLANNING_STD_EXTENSIONS_H
#define PLANNING_STD_EXTENSIONS_H

namespace std
{
    template <typename T, typename U>
    struct hash<std::pair<T, U>>
    {
        size_t operator()(const std::pair<T, U>& pair) const
        {
            std::hash<T> firstHashFunc;
            std::hash<U> secondHashFunc;

            size_t seed = 0;
            seed ^= firstHashFunc(pair.first) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= secondHashFunc(pair.second) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

            return seed;
        }
    };
}

#endif /* PLANNING_STD_EXTENSIONS_H */