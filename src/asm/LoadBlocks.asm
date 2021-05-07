
public _LoadBlocks
extern _ti_Open
extern _ti_GetDataPtr
extern _ti_Close
extern _sprites

_LoadBlocks:
	pop bc,hl
	push hl,bc
	ld bc,__LoadBlocks_mode
	push bc,hl
	call _ti_Open
	or a,a
	pop bc,bc
	ret z
	ld (__LoadBlocks_smc),a
	ld c,a
	push bc
	call _ti_GetDataPtr
	inc hl
	inc hl
	inc hl
	inc hl
	pop bc
	ld de,_sprites
	ld bc,258
	ld a,64
__LoadBlocks_loop:
	ex hl,de
	ld (hl),de
	inc hl
	inc hl
	inc hl
	ex hl,de
	add hl,bc
	dec a
	jr nz,__LoadBlocks_loop
	ld c,0
__LoadBlocks_smc:=$-1
	push bc
	call _ti_Close
	pop bc
	ret
__LoadBlocks_mode:
	db "r",0

