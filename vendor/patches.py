import sys
import re
from pathlib import Path

class Patcher:
	def __init__(self, filePath: str):
		self.filePath = Path(filePath)
		if not self.filePath.exists():
			raise FileNotFoundError(f"File not found: {filePath}")
		self.lines = self.filePath.read_text().splitlines()

	def showPatchInfo(self, patchBlock: dict):
		print("- PATCH INFO -")
		if patchBlock.get("comment"):
			print("* COMMENT:", patchBlock["comment"])
		if patchBlock.get("note"):
			print("* NOTE:", patchBlock["note"])
		if patchBlock.get("change"):
			for change in patchBlock["change"]:
				print(f"* CHANGE: {change['from']} -> {change['to']}")
		print("----------------")

	def applyPatch(self, patchDate: str, showInfo: bool = False):
		i = 0
		while i < len(self.lines):
			line = self.lines[i]
			m = re.match(r"// PATCH \((.*?)\):", line)
			if m:
				foundDate = m.group(1)
				patchBlock = {"comment": "", "note": "", "change": []}
				i += 1

				while i < len(self.lines):
					currentLine = self.lines[i].strip()

					if currentLine.endswith(");"):
						break

					# COMMENT block (can span multiple lines)
					if currentLine.startswith("// * COMMENT:"):
						j = i + 1
						while j < len(self.lines):
							nextLine = self.lines[j].strip()
							if nextLine.startswith("// * NOTE:") or nextLine.startswith("// * CHANGE:") or nextLine.endswith(");"):
								break
							if nextLine.startswith("// *"):
								patchBlock["comment"] += " " + nextLine[3:].strip()
							j += 1
						i = j - 1

					# NOTE block (can span multiple lines)
					elif currentLine.startswith("// * NOTE:"):
						j = i + 1
						while j < len(self.lines):
							nextLine = self.lines[j].strip()
							if nextLine.startswith("// * COMMENT:") or nextLine.startswith("// * CHANGE:") or nextLine.endswith(");"):
								break
							if nextLine.startswith("// *"):
								patchBlock["note"] += " " + nextLine[3:].strip()
							j += 1
						i = j - 1

					# CHANGE block
					elif currentLine.startswith("// * CHANGE:"):
						j = i + 1
						while j < len(self.lines):
							changeLine = self.lines[j].strip()
							if changeLine.startswith("//"):
								changeLine = changeLine[2:].strip()
							if "@TO@" in changeLine:
								parts = changeLine.split("@TO@")
								patchBlock["change"].append({
									"from": parts[0].strip(),
									"to": parts[1].strip()
								})
								j += 1
							else:
								break
						i = j - 1

					i += 1

				# show patch info
				if showInfo:
					self.showPatchInfo(patchBlock)

				# apply patch
				if foundDate == patchDate and patchBlock["change"]:
					for change in patchBlock["change"]:
						for j in range(len(self.lines)):
							if self.lines[j].lstrip().startswith(change["from"]):
								self.lines[j] = self.lines[j].replace(change["from"], change["to"])

			i += 1

		self.filePath.write_text("\n".join(self.lines))
		print(f"Patched file saved: {self.filePath}")

if __name__ == "__main__":
	if len(sys.argv) < 3:
		print("Usage: python patcher.py <filePath> <patchDate> [showInfo]")
		sys.exit(1)

	filePath = sys.argv[1]
	patchDate = sys.argv[2]
	showInfo = len(sys.argv) > 3 and sys.argv[3].lower() == "true"

	patcher = Patcher(filePath)
	patcher.applyPatch(patchDate, showInfo)

