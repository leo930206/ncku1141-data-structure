import sys
from PyPDF2 import PdfReader

if len(sys.argv) < 3:
    print("Usage: extract_pdf.py input.pdf output.txt")
    sys.exit(1)

inpath = sys.argv[1]
outpath = sys.argv[2]

reader = PdfReader(inpath)
with open(outpath, 'w', encoding='utf-8') as f:
    for i, page in enumerate(reader.pages):
        try:
            text = page.extract_text()
        except Exception as e:
            text = ''
        if text:
            f.write(text)
            f.write('\n---PAGE %d END---\n' % (i+1))
print('WROTE', outpath)
