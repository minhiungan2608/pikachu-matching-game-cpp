#!/usr/bin/env python3
"""Generate the numbered-tile demo media using only Python's standard library."""
from pathlib import Path
import colorsys
import math
import struct
import wave
import zlib

ROOT = Path(__file__).resolve().parents[1] / "data"
FONT = {
"A":["010","101","111","101","101"],"B":["110","101","110","101","110"],
"C":["011","100","100","100","011"],"D":["110","101","101","101","110"],
"E":["111","100","110","100","111"],"F":["111","100","110","100","100"],
"G":["011","100","101","101","011"],"H":["101","101","111","101","101"],
"I":["111","010","010","010","111"],"J":["001","001","001","101","010"],
"K":["101","101","110","101","101"],"L":["100","100","100","100","111"],
"M":["101","111","111","101","101"],"N":["101","111","111","111","101"],
"O":["010","101","101","101","010"],"P":["110","101","110","100","100"],
"Q":["010","101","101","111","011"],"R":["110","101","110","101","101"],
"S":["011","100","010","001","110"],"T":["111","010","010","010","010"],
"U":["101","101","101","101","111"],"V":["101","101","101","101","010"],
"W":["101","101","111","111","101"],"X":["101","101","010","101","101"],
"Y":["101","101","010","010","010"],"Z":["111","001","010","100","111"],
"0":["111","101","101","101","111"],"1":["010","110","010","010","111"],
"2":["110","001","010","100","111"],"3":["110","001","010","001","110"],
"4":["101","101","111","001","001"],"5":["111","100","110","001","110"],
"6":["011","100","111","101","111"],"7":["111","001","010","010","010"],
"8":["111","101","111","101","111"],"9":["111","101","111","001","110"],
" ":["000"]*5,"-":["000","000","111","000","000"]
}

class Image:
    def __init__(self, w, h, color):
        self.w, self.h = w, h
        self.pixels = bytearray(bytes(color) * (w*h))
    def rect(self, x, y, w, h, color):
        for row in range(max(0,y), min(self.h,y+h)):
            start = (row*self.w+max(0,x))*3
            end = (row*self.w+min(self.w,x+w))*3
            if end > start:
                self.pixels[start:end] = bytes(color) * ((end-start)//3)
    def text(self, text, x, y, scale=3, color=(235,244,250)):
        for char in text.upper():
            for row, pixels in enumerate(FONT[char]):
                for col, bit in enumerate(pixels):
                    if bit == "1": self.rect(x+col*scale,y+row*scale,scale,scale,color)
            x += 4*scale
    def center(self, text, y, scale=3, color=(235,244,250)):
        self.text(text, (self.w-(len(text)*4-1)*scale)//2, y, scale, color)
    def save(self, path):
        path.parent.mkdir(parents=True, exist_ok=True)
        def chunk(kind, data):
            return struct.pack(">I",len(data))+kind+data+struct.pack(">I",zlib.crc32(kind+data)&0xffffffff)
        raw = b"".join(b"\0"+self.pixels[y*self.w*3:(y+1)*self.w*3] for y in range(self.h))
        path.write_bytes(b"\x89PNG\r\n\x1a\n"+
            chunk(b"IHDR",struct.pack(">IIBBBBB",self.w,self.h,8,2,0,0,0))+
            chunk(b"IDAT",zlib.compress(raw,9))+chunk(b"IEND",b""))

def screen(title, playing=False):
    img = Image(1200,800,(16,27,44))
    img.rect(0,0,1200,8,(42,205,181))
    img.center(title,90,6)
    if playing:
        img.rect(180,136,840,476,(32,48,67))
        img.text("MATCH IDENTICAL TILES",184,650,4)
        img.text("PATHS MAY TURN AT MOST TWICE",184,690,3)
    else:
        img.center("C PLUS PLUS - SDL2",210,4,(42,205,181))
        img.center("NUMBERED TILE DEMO",260,3)
    return img

def tone(path, freq, duration):
    path.parent.mkdir(parents=True,exist_ok=True)
    rate=22050
    samples = bytearray()
    for i in range(int(rate*duration)):
        value=int(2200*math.sin(2*math.pi*freq*i/rate)*(1-i/(rate*duration))) if freq else 0
        samples.extend(struct.pack("<h",value))
    with wave.open(str(path),"wb") as f:
        f.setnchannels(1); f.setsampwidth(2); f.setframerate(rate); f.writeframes(samples)

def main():
    for n in range(1,25):
        rgb=tuple(int(v*255) for v in colorsys.hsv_to_rgb((n-1)/24,0.45,0.9))
        for state in ("up","down"):
            img=Image(52,52,(42,205,181) if state=="down" else (16,27,44))
            img.rect(3,3,46,46,rgb)
            img.center(str(n),16,4,(16,27,44))
            img.save(ROOT/"pokemon_icon"/f"{n}-{state}.png")
    screen("PIKACHU MATCHING GAME").save(ROOT/"pokemon_screen"/"mewtwo.png")
    for level in range(1,8):
        screen(f"LEVEL {level}",True).save(ROOT/"pokemon_screen"/f"{level}.png")
        for suffix,label in (("pause","PAUSED"),("sub-win","LEVEL COMPLETE"),("sub-lose","TRY AGAIN")):
            screen(f"LEVEL {level} - {label}").save(ROOT/"pokemon_screen"/f"{level}-{suffix}.png")
    for name,label in (("play","PLAY"),("main_menu","MAIN MENU"),("new","NEW"),
        ("pause","PAUSE"),("menu","MENU"),("continue","CONTINUE"),
        ("next_level","NEXT LEVEL"),("play_again","PLAY AGAIN")):
        img=Image(200,100,(42,205,181)); img.center(label,40,4,(16,27,44))
        img.save(ROOT/"pokemon_screen"/f"{name}_button.png")
    for n in range(8):
        img=Image(100,75,(32,48,67));img.center("SHUFFLES",12,2);img.center(str(n),34,5)
        img.save(ROOT/"chance"/f"{n}-chance.png")
    Image(2,30,(42,205,181)).save(ROOT/"time"/"run_time.png")
    for name,freq,duration in (("first_move",440,0.09),("delete",660,0.16),
        ("no_delete",220,0.18),("soundtrack",0,1.0)):
        tone(ROOT/"audio"/f"{name}.wav",freq,duration)
    print("Generated 94 demo PNGs and 4 WAV files; soundtrack is intentionally silent.")

if __name__=="__main__": main()
