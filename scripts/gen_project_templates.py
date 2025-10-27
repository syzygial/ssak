import glob
import os
import sys

def gen_project_templates():
  template_dirs = glob.glob('src/templates/*')
  sys.stdout.write("// This section generated automatically by scripts/gen_project_templates.py\n\n")
  sys.stdout.write("namespace {\n\n")
  for d in template_dirs:
    sys.stdout.write("static const project_template {}_project {{".format(d.split('/')[-1]))
    # exclude directories, courtesy of
    # https://stackoverflow.com/a/42327727
    template_files = [x for x in glob.glob(d + '/**', recursive=True) if not os.path.isdir(x)]
    if len(template_files) == 0:
      sys.stdout.write("};\n\n")
    else:
      sys.stdout.write("\n")
      for fname in template_files:
        relative_fname = '/'.join(fname.split('/')[3:])
        sys.stdout.write('  {{"{}",\n'.format(relative_fname))
        with open(fname, 'r') as f:
          for line in f:
            sys.stdout.write('  "{}\\n"\n'.format(line.strip('\n')
              .replace('"', R'\"')
              .replace(R'\n', R'\\n')))
        sys.stdout.write("  },\n")
      sys.stdout.write("};\n\n")
  sys.stdout.write("static const std::map<std::string, project_template> templates {\n")
  for d in template_dirs:
    sys.stdout.write('  {{"{0}", {0}_project}},\n'.format(d.split('/')[-1]))
  sys.stdout.write("};\n\n")
  sys.stdout.write("} // namespace\n\n")
  sys.stdout.write("// End automatically-generated section\n")
if __name__ == "__main__":
  gen_project_templates()