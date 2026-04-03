#pragma once

namespace nil::xalt
{
    template <typename T>
    class raii
    {
    public:
        explicit raii(T* init_object, void (*init_cleanup)(T*))
            : object(init_object)
            , cleanup(init_cleanup)
        {
        }

        raii(raii&& o) noexcept = delete;
        raii(const raii& o) = delete;

        raii& operator=(const raii& o) = delete;
        raii& operator=(raii&& o) = delete;

        ~raii()
        {
            if (cleanup != nullptr)
            {
                cleanup(object);
            }
        }

        T* object;
        void (*cleanup)(T*);
    };
}
