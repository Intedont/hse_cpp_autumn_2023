from setuptools import setup, Extension

def main():
    setup(
        name="cjson",
        author="M.A.Dusov",
        version="0.0.1",
        # флаг -Werror удален, т.к. он кидает ошибки на неиспользованный параметр self
        ext_modules=[Extension('cjson', ['cjson.c'], 
                               extra_compile_args = ['-Wunused-variable', '-Wall', 
                                                     '-Wextra', '-Wsign-conversion' 
                                                     ],
                                extra_link_args=['-fsanitize=address'])] 
    )

if __name__ == "__main__":
    main()