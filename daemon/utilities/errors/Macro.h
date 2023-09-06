#define BXT_DECLARE_RESULT(error) \
    template<typename T> using Result = tl::expected<T, error>;