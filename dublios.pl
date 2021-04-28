#!/usr/bin/env perl

# dublios.pl [<папка с файлами>]

# Формат виходу програми має бути таким:
# <назва файлу><кома><довжина неперіодичної частини (передперіод, тобто)><кома><довжина періодичної, повторюваної частини>.

$| = 1;

$dir = $ARGV[0] // "dublios-data";

# получаем файлы в папке
while(<$dir/*>) {
	next if !-f;
	open f, $_ or die "$_: $!"; # открываем файл

	print $_, ","; # печатаем название файла

	# считываем файл в $s
	$size = -s f; # размер файла
	read f, $s, $size;
	close f;

	# ищем период повторяющийся трижды
	$period = "";
	for $i (2..1024) {
		last if ($period) = $s =~ /(.{$i})\1\1\z/s;
	}

	# если период не найден
	print("-\n"), next if $period eq "";

	# ищем повторы
	$quoted_period = quotemeta $period;
	print("-\n"), next if $s !~ /(?:$quoted_period)+$/s;

	# длина повторений
	$len = length $&;

	# захватываем символы, которые совпадают с концом периода и находятся перед повторениями
	$c = 0;
	@period = map quotemeta, split //, $period;
	for($i=$#period; $i>=0; $i--) {
		$quoted_period = join "", @period[$i..$#period];
		pos($s) = $size - $len;
		last if $s !~ /$quoted_period\G/;
		$c++;
	}

	$len += $c;

	# длина перед повторениями
	print $size - $len, ",";

	# длина повторений
	print $len, "\n";
}