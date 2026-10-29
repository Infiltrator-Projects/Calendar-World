#!/usr/bin/env python3
"""Create the illustrated public calendar guide from provider-owned facts."""
import json
import sys
from pathlib import Path
from reportlab.lib.colors import HexColor
from reportlab.lib.pagesizes import A4
from reportlab.lib.utils import simpleSplit
from reportlab.pdfbase import pdfmetrics
from reportlab.pdfbase.ttfonts import TTFont
from reportlab.pdfgen import canvas

W,H=A4
INK=HexColor("#22334A")
RUST=HexColor("#B45C36")
FAINT=HexColor("#64788C")
PALE=HexColor("#F1ECE3")
BG=HexColor("#FAF8F3")
FONT="Helvetica"
BOLD="Helvetica-Bold"
for name,file in [("Guide","DejaVuSans.ttf"),("GuideBold","DejaVuSans-Bold.ttf")]:
    file=Path("/usr/share/fonts/truetype/dejavu")/file
    if file.exists():
        pdfmetrics.registerFont(TTFont(name,str(file)))
if "Guide" in pdfmetrics.getRegisteredFontNames():
    FONT,BOLD="Guide","GuideBold"
def draw(c,s,x,y,size=10,bold=False,color=INK):
    c.setFillColor(color);c.setFont(BOLD if bold else FONT,size);c.drawString(x,y,str(s))
def para(c,s,x,y,width,size=10,step=18):
    c.setFont(FONT,size);c.setFillColor(INK)
    for paragraph in str(s).split("\n"):
        for line in simpleSplit(paragraph,FONT,size,width):
            c.drawString(x,y,line);y-=step
    return y
def page(c,index,title):
    c.setFillColor(BG);c.rect(0,0,W,H,fill=1,stroke=0)
    draw(c,"INFILTRATOR  /  CALENDAR LIBRARY",48,H-37,8,True,FAINT)
    draw(c,title,48,H-103,22,True)
    c.setStrokeColor(PALE);c.line(48,49,W-48,49)
    draw(c,"ILLUSTRATED CALENDAR GUIDE",48,30,8,False,FAINT)
    draw(c,f"{index} / 3",W-78,30,8,False,FAINT)
def ring(c,x,y,month_count=12,count_label=None,caption="PERIODS"):
    import math
    c.setStrokeColor(RUST);c.setLineWidth(6);c.circle(x,y,55,fill=0,stroke=1)
    for k in range(month_count):
        a=math.pi*2*k/month_count
        c.setStrokeColor(FAINT);c.setLineWidth(1)
        c.line(x+48*math.cos(a),y+48*math.sin(a),
               x+66*math.cos(a),y+66*math.sin(a))
    draw(c,str(count_label or month_count),x-37,y+2,24,True)
    draw(c,caption,x-23,y-18,8,True,FAINT)
def main():
    data=json.loads(Path(sys.argv[1]).read_text(encoding="utf-8"))
    assert 4 <= len(data["sections"]) <= 16, "Guide requires 4-16 illustrated periods"
    dest=Path(sys.argv[2]);dest.parent.mkdir(parents=True,exist_ok=True)
    c=canvas.Canvas(str(dest),pagesize=A4,invariant=1)
    c.setTitle(data["title"]+" - Illustrated guide")
    c.setAuthor("Infiltrator Projects")
    page(c,1,"UNDERSTANDING THE CALENDAR")
    draw(c,data["title"],48,H-160,14,True,RUST)
    para(c,data["intro"],48,H-201,306,10.5,19)
    ring(c,W-118,H-249,len(data["sections"]),data.get("ring_label"),data.get("ring_caption","PERIODS"))
    c.setFillColor(PALE);c.roundRect(48,H-460,W-96,112,10,fill=1,stroke=0)
    draw(c,"AT A GLANCE",66,H-379,10,True,RUST)
    para(c,data["summary"],66,H-406,W-134,11,19)
    draw(c,"WHERE IT COMES FROM",48,H-527,11,True,RUST)
    para(c,data["origin"],48,H-559,W-96,10.5,19)
    draw(c,"A visual guide to the calendar, not a programming manual.",48,92,9,False,FAINT)
    c.showPage()
    page(c,2,data["diagram_title"])
    para(c,data["structure"],48,H-146,W-96,10.5,18)
    entries=data["sections"]
    x,y,w=48,H-302,W-96
    for i,e in enumerate(entries):
        left=x+i*w/len(entries)
        c.setFillColor(RUST if e.get("special") else (INK if i%2 else FAINT))
        c.rect(left,y,w/len(entries)-1,43,fill=1,stroke=0)
        draw(c,str(i+1),left+max(3,w/len(entries)/3),y+15,8 if len(entries)==12 else 6.5,True,HexColor("#FFFFFF"))
    draw(c,data["diagram_caption"],48,y-27,9,True,RUST)
    draw(c,"READING THE PATTERN",48,H-386,11,True)
    cols=3 if len(entries)>12 else 2
    rows=(len(entries)+cols-1)//cols
    cw=(W-96)/cols
    top=H-417
    for i,e in enumerate(entries):
        col=i//rows;row=i%rows
        xx=48+col*cw;yy=top-row*29
        c.setFillColor(PALE);c.roundRect(xx,yy-9,cw-8,25,4,fill=1,stroke=0)
        label=str(e["name"])
        if len(label)>19: label=label[:18]+"…"
        draw(c,label,xx+7,yy+1,7.7 if cols==3 else 8.8,True,INK)
        val=str(e.get("days",""))
        if val: draw(c,val,xx+cw-11-pdfmetrics.stringWidth(val,FONT,7),yy+1,7,False,RUST)
    note_y=max(110,top-rows*29-35)
    draw(c,"WHY THIS MATTERS",48,note_y+18,10,True,RUST)
    para(c,data["pattern_note"],48,note_y-6,W-96,9.5,16)
    c.showPage()
    page(c,3,"HISTORY AND REAL-WORLD USE")
    draw(c,"HISTORICAL CONTEXT",48,H-146,11,True,RUST)
    para(c,data["history"],48,H-176,W-96,10.5,19)
    c.setStrokeColor(FAINT);c.setLineWidth(5);c.line(83,H-338,W-83,H-338)
    c.setStrokeColor(RUST);c.line(83,H-383,W-83,H-383)
    for i in range(4):
        xx=83+i*(W-166)/3
        c.setFillColor(INK);c.circle(xx,H-338,4,fill=1,stroke=0)
        c.setFillColor(RUST);c.circle(xx+(i-1)*5,H-383,4,fill=1,stroke=0)
    draw(c,"CALENDAR RULE  /  COMPUTED MODEL",80,H-319,8,True,FAINT)
    draw(c,"LOCAL PRACTICE  /  HISTORICAL REALITY",80,H-410,8,True,RUST)
    draw(c,"AN EXAMPLE",48,H-478,11,True,RUST)
    para(c,data["example"],48,H-506,W-96,10,17)
    draw(c,"IMPORTANT LIMITATIONS",48,H-598,11,True,RUST)
    para(c,data["limits"],48,H-625,W-96,9.2,15)
    draw(c,"Sources and implementation:",48,107,8,True)
    para(c,data["source"],48,90,W-96,7.5,13)
    c.save()
if __name__=="__main__":
    main()
