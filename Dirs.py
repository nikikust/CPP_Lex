import os
path = '.'
list_files = []
skip = ['profile.h', 'InitJSON.h']
for root, dirs, files in os.walk(path):
    for file in files:
        if os.path.splitext(file)[1] in [".h", ".cpp"] and file not in skip:
            list_files.append(os.path.join(root,file)[2:])

out = ' '.join(list_files)
print(out)
