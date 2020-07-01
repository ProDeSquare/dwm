void
fibonacci(Monitor *mon, int s) {
	unsigned int i, n, nx, ny, nw, nh;
	int oh, ov, ih, iv;
	Client *c;

	for(n = 0, c = nexttiled(mon->clients); c; c = nexttiled(c->next), n++);

	getgaps(mon, &oh, &ov, &ih, &iv, &n);

	if(n == 0)
		return;

	nx = mon->wx + ov;
	ny = oh;
	nw = mon->ww - 2*ov;
	nh = mon->wh - 2*oh;

	for(i = 0, c = nexttiled(mon->clients); c; c = nexttiled(c->next)) {
		if((i % 2 && nh / 2 > 2 * c->bw)
		   || (!(i % 2) && nw / 2 > 2 * c->bw)) {
			if(i < n - 1) {
				if(i % 2)
					nh = (nh - ih) / 2;
				else
					nw = (nw - iv) / 2;
				if((i % 4) == 2 && !s)
					nx += nw + iv;
				else if((i % 4) == 3 && !s)
					ny += nh + ih;
			}
			if((i % 4) == 0) {
				if(s)
					ny += nh + ih;
				else
					ny -= nh + ih;
			}
			else if((i % 4) == 1)
				nx += nw + iv;
			else if((i % 4) == 2)
				ny += nh + ih;
			else if((i % 4) == 3) {
				if(s)
					nx += nw + iv;
				else
					nx -= nw + iv;
			}
			if(i == 0)
			{
				if(n != 1)
					nw = mon->ww * mon->mfact;
				ny = mon->wy + oh;
			}
			else if(i == 1)
				nw = mon->ww - nw - iv - 2 * ov;
			i++;
		}

		resize(c, nx, ny, nw - 2 * c->bw, nh - 2 * c->bw, False);
	}
}

void
dwindle(Monitor *mon) {
	fibonacci(mon, 1);
}

void
spiral(Monitor *mon) {
	fibonacci(mon, 0);
}
