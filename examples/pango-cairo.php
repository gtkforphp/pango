<?php
/* This is a port of the equivalent code from the PangoCairo documentation */
define("RADIUS", 150);
define("N_WORDS", 10);
define("FONT", "Sans Bold 27");

function draw_text($c) {
    $c->translate(RADIUS, RADIUS);
    $l = new PangoLayout($c);
    $l->setText("Text");    
    $desc = new PangoFontDescription(FONT);
    $l->setFontDescription($desc);

    for($i = 0; $i < N_WORDS; $i++) {
        $angle = 360.0 * $i / N_WORDS;
        $red = (1 + cos (($angle - 60) * M_PI / 180)) / 2;

        $c->save();
        $c->setSourceRGB($red, 0, 1.0 - $red);
        $c->rotate($angle * M_PI / 180.0);
        $l->updateLayout($c);
        $size = $l->getSize();
        $c->moveTo( - ((double)$size['width'] / 2048), - RADIUS);
        $l->showLayout($c);
        $c->restore();
    }
}

$s = new CairoImageSurface(CairoFormat::ARGB32, RADIUS *2, RADIUS *2);
$c = new CairoContext($s);
$c->setSourceRGB(1, 1, 1);
$c->paint();
draw_text($c);
$s->writeToPng('circle.png');
?>
