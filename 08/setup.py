from setuptools import setup, Extension

# Убрал флаги, потому что они ломают всю компиляцию
# флаг -Werror удален, т.к. он кидает ошибки на неиспользованный параметр self

#compile_flags = ['-Wunused-variable', '-Wall', '-Wextra', '-Wsign-conversion', '-fsanitize=address']

def main():
    setup(
        name="cjson",
        author="M.A.Dusov",
        version="0.0.1",

        ext_modules=[Extension('cjson', 
                               ['cjson.c'],
                               #extra_compile_args=compile_flags,
                               #extra_link_args=['-fsanitize=address']
                               )])


if __name__ == "__main__":
    main()