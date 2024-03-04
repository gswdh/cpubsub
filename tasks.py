from invoke import task

@task
def clang_format(c):
    c.run('find . -type f \( -name "*.c" -o -name "*.h" \) -exec clang-format -i -style=file {} \;')
    print("Done!")
