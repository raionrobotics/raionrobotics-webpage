<svelte:head>
	<title>Real Estate Svelte & TailwindCSS Template: Modern Home & Property Listings</title>
	<meta
		name="description"
		content="Real Estate Template with Svelte and TailwindCss"
	/>
</svelte:head>

<section id="hero" style="background-image: url('/images/RaiboHill.jpg'); background-size: cover; height:680px; padding-top:80px;">
	<div class="grid max-w-screen-xl py-8 mx-auto lg:gap-8 xl:gap-0 lg:py-0 lg:pt-16 lg:grid-cols-12">
		<div class="mr-auto place-self-center lg:col-span-7">
			<h1
				class="indent-8 max-w-2xl mb-4 text-4xl text-white font-extrabold tracking-tight leading-none md:text-5xl xl:text-6xl"
			>
				라이보
			</h1>
			<p class="indent-8 max-w-2xl mb-6 font-bold text-white lg:mb-8 md:text-lg lg:text-xl">
				모든 지형을 위한 사족 로봇 솔루션
			</p>

		</div>
	</div>
</section>

<section id="service" class="px-6 bg-white py-12">
	<h2 class="font-bold text-2xl md:text-4xl lg:text-5xl text-center">Service</h2>
	<div
		class="grid max-w-screen-xl grid-cols-1 place-items-center px-4 py-8 mx-auto gap-8 xl:gap-0 lg:py-16 md:grid-cols-3"
	>
		<div class="flex flex-col space-y-2 items-start px-6 py-6">
			<svg
				xmlns="http://www.w3.org/2000/svg"
				width="55"
				height="58"
				viewBox="0 0 55 58"
				fill="none"
				class="text-indigo-500"
			>
		</div>		
	</div>
</section>

<section id="contact" class="px-6 py-12 bg-gray-100">
	<div class="grid max-w-screen-xl grid-cols-1 px-4 py-8 mx-auto gap-3 lg:py-16 md:grid-cols-2">
		<img
			src="/images/house2.png"
			class="h-full lg:w-10/12"
			width="996"
			height="662"
			alt="A House"
		/>
		<div>
			<h3 class="font-bold text-2xl">Find the right one for you!</h3>
			<p class="font-light text-sm py-2 mb-3 text-gray-500">
				Schedule a consultation with an agent
			</p>
			<form>
				<div class="mb-6 flex flex-col md:flex-row gap-5">
					<input
						type="text"
						id="firstName"
						class="bg-gray-50 border border-gray-300 text-gray-900 text-sm rounded-lg focus:ring-purple-500 focus:border-purple-500 block w-full p-2.5"
						placeholder="First Name"
						required
					/>
					<input
						type="text"
						id="lastName"
						class="bg-gray-50 border border-gray-300 text-gray-900 text-sm rounded-lg focus:ring-purple-500 focus:border-purple-500 block w-full p-2.5"
						placeholder="Last Name"
						required
					/>
				</div>
				<div class="mb-6 flex flex-col md:flex-row gap-5">
					<input
						type="email"
						id="email"
						class="bg-gray-50 border border-gray-300 text-gray-900 text-sm rounded-lg focus:ring-purple-500 focus:border-purple-500 block w-full p-2.5"
						placeholder="Email address"
						required
					/>
					<input
						type="tel"
						id="phone"
						class="bg-gray-50 border border-gray-300 text-gray-900 text-sm rounded-lg focus:ring-purple-500 focus:border-purple-500 block w-full p-2.5"
						placeholder="Phone Number"
						required
					/>
				</div>
				<textarea
					id="comment"
					rows="4"
					class="w-full px-3 py-3 text-sm text-gray-900 bg-gray-50 border rounded-lg border-gray-300 focus:ring-0"
					placeholder="Description"
					required
				/>
				<p class="font-light text-gray-500 mb-6">We don't send you spam</p>
				<button
					type="button"
					class="w-full text-white bg-purple-700 hover:bg-purple-800 focus:ring-4 focus:outline-none focus:ring-purple-300 font-medium rounded-lg text-sm px-4 py-4 text-center mr-3 md:mr-0"
					>Contact Us</button
				>
			</form>
		</div>
	</div>
</section>
